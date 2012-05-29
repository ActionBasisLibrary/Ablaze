//
//  GMatrixSparse.cpp
//  GMath
//
//  Created by Caleb Jordan on 11/14/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GMatrixSparse.h"

#include <map>
#include <algorithm>

// Predecs

static bool elmSort(const gMatrixSparse::elm &one, const gMatrixSparse::elm &two);

inline static bool coordSort(int row1, int col1, int row2, int col2)
{ return (col1 < col2 || (col1 == col2 && row1 < row2)); }

#pragma mark CONSTRUCTORS

gMatrixSparse::gMatrixSparse(int rows, int cols)
: numRows(rows), numCols(cols), nnz(0)
{
    init();
}

gMatrixSparse::gMatrixSparse(int rows, int cols, int nnz)
: numRows(rows), numCols(cols), nnz(nnz)
{
    init();
}

gMatrixSparse::gMatrixSparse(int rows, int cols, vector<elm> &elements, bool sorted)
: numRows(rows), numCols(cols), nnz(0)
{
    init();
    fill(elements, sorted);
}

gMatrixSparse::~gMatrixSparse()
{
    
}

#pragma mark EFFICIENT ACCESS

void gMatrixSparse::allocate(int numnz)
{
    nnz = numnz;
    vals = vector<double>(nnz);
    rows = vector<int>(nnz);
    cols[numCols] = nnz;
}

void gMatrixSparse::fill(vector<elm> &vect, bool sorted)
{   
    if (!sorted) sort(vect.begin(), vect.end(), elmSort);
    
    allocate((int)vect.size());
    
    int col = -1;
    for (int i = 0, j = 0; i < vect.size(); i++, j++) {
        double val = vect[i].val;
        
        if (fabs(val) < eps) {
            nnz--; j--;
            continue;
        }
        
        if (vect[i].col > col) {
            for (int c = col + 1; c <= vect[i].col; c++) {
                cols[c] = j;
            }
            col = vect[i].col;
        }
        
        vals[j] = vect[i].val;
        rows[j] = vect[i].row;
    }
    
    for (int c = col + 1; c <= numCols; c++)
        cols[c] = nnz;
}

#pragma mark RANDOM ACCESS IS DUMB

double gMatrixSparse::at(int row, int col)
{
    int i;
    if (getIndex(row, col, i)) return vals[i];
    else return 0.0;
}

void gMatrixSparse::set(int row, int col, double value)
{
    int i;
    if (getIndex(row, col, i)) {
        insertRandom(row, col, value, i);
    } else {
        if (fabs(value) < eps) {
            removeRandom(col, i);
        } else {
            vals[i] = value;
        }
    }
}

void gMatrixSparse::map(mapFn function, const void *stuff)
{
    for (int c = 0; c < numCols; c++) {
        for (int i = cols[c]; i < cols[c + 1]; i++) {
            function(rows[i], c, vals[i], stuff);
        }
    }
}

#pragma mark OPERATORS

gMatrixSparse gMatrixSparse::operator+(gMatrixSparse &right)
{    
    struct miniMapper : public dualMapper {
        vector<elm> vect;
        virtual void map(int row, int col, double valLeft, double valRight) {
            vect.push_back(elm(row, col, valLeft + valRight));
        }
    } mapper;
    
    gMatrixSparse::dualMap(*this, right, mapper);
    
    return gMatrixSparse(numRows, numCols, mapper.vect, true);
}

gMatrixSparse gMatrixSparse::operator-(gMatrixSparse &right)
{    
    struct miniMapper : public dualMapper {
        vector<elm> vect;
        virtual void map(int row, int col, double valLeft, double valRight) {
            vect.push_back(elm(row, col, valLeft - valRight));
        }
    } mapper;
    
    gMatrixSparse::dualMap(*this, right, mapper);
    
    return gMatrixSparse(numRows, numCols, mapper.vect, true);
}

gMatrixSparse gMatrixSparse::operator*(gMatrixSparse &right)
{
    assert(numCols == right.numRows);
    
    vector<elm> vect;
    
    gMatrixSparse trans = getTranspose();
    bool nempty[numRows];
    std::map<int, gMatrixSparse::gColumnSparse> rowMap;
    for (int i = 0; i < numRows; i++) {
        nempty[i] = !trans.colIsEmpty(i);
        if (nempty[i]) rowMap[i] = trans.getColumn(i);
    }
    
    for (int c = 0; c < right.numCols; c++) {
        if (right.colIsEmpty(c)) continue;
        gColumnSparse col = right.getColumn(c);
        for (int r = 0; r < numRows; r++) {
            if (nempty[r]) {
                double val = col.dot(rowMap[r]);
                vect.push_back(elm(r, c, val));
            }
        }
    }
    
    return gMatrixSparse(numRows, right.numCols, vect, true);
}

gMatrixSparse gMatrixSparse::operator*(double scalar)
{
    gMatrixSparse result = *this;
    result *= scalar;
    return result;
}

void gMatrixSparse::operator*=(double scalar)
{
    for (int i = 0; i < nnz; i++)
        vals[i] *= scalar;
}

#pragma mark CLEVER STUFF

static void transMap(const int row, const int col, double &value, const void *stuff)
{
    ((vector<gMatrixSparse::elm> *)stuff)->push_back(gMatrixSparse::elm(col, row, value));
}

gMatrixSparse gMatrixSparse::getTranspose()
{
    vector<elm> vect;
    map(transMap, &vect);
    return gMatrixSparse(numRows, numCols, vect, false);
}

gMatrixSparse gMatrixSparse::getNearSymmetric()
{
    gMatrixSparse trans = getTranspose();    
    
    struct miniMapper : public dualMapper {
        vector<elm> vect;
        virtual void map(int row, int col, double valLeft, double valRight) {
            vect.push_back(elm(row, col, (valLeft + valRight) * .5));
        }
    } mapper;
    
    gMatrixSparse::dualMap(*this, trans, mapper);
    
    return gMatrixSparse(numRows, numCols, mapper.vect, true);
}

bool gMatrixSparse::isSymmetric()
{
    if (numRows != numCols) return false;
    
    return (getTranspose() - *this).nnz == 0;
}

#pragma mark STATIC STUFF

void gMatrixSparse::dualMap(gMatrixSparse &left, gMatrixSparse &right, dualMapper &mapper)
{
    assert(left.numRows == right.numRows && left.numCols == right.numCols);
    
    iterator lit(left), rit(right);
    while (lit.good() || rit.good()) {
        bool both = lit.good() && rit.good();
        if (!rit.good() || (both && lit < rit)) {
            mapper.map(lit.row, lit.col, lit.val, 0.0);
            lit.next();
        } else if (both && lit == rit) {
            mapper.map(lit.row, lit.col, lit.val, rit.val);
            lit.next();
            rit.next();
        } else {
            mapper.map(rit.row, rit.col, 0.0, rit.val);
            rit.next();
        }
    }
}

gMatrixSparse gMatrixSparse::getIdentity(int numRows)
{
    return getIdentity(numRows, numRows);
}

gMatrixSparse gMatrixSparse::getIdentity(int numRows, int numCols)
{
    vector<elm> vect;
    
    int d = min(numRows, numCols);
    for (int i = 0; i < d; i++)
        vect.push_back(elm(i, i, 1.0));
    
    return gMatrixSparse(numRows, numCols, vect);
}

gMatrixSparse gMatrixSparse::eVect(int index, int length, bool transpose)
{
    vector<elm> vect;
    if (transpose) {
        vect.push_back(elm(0, index - 1, 1.0));
        return gMatrixSparse(1, length, vect);
    } else {
        vect.push_back(elm(index - 1, 0, 1.0));
        return gMatrixSparse(length, 1, vect);
    }
}

#pragma mark PRIVATE

void gMatrixSparse::init()
{
    eps = 1e-12;
    vals = vector<double>(nnz);
    rows = vector<int>(nnz);
    cols = vector<int>(numCols + 1);
    
    for (int i = 0; i < numCols; i++)
        cols[i] = 0;
    cols[numCols] = nnz;
}

#pragma mark RANDOM ACCESS HELPERS

bool gMatrixSparse::getIndex(int row, int col, int &index)
{
    int i;
    for (i = cols[col]; i < cols[col + 1]; i++) {
        if (rows[i] == row) {
            index = i;
            return false;
        }
    }
    index = i;
    return true;
}

void gMatrixSparse::insertRandom(int row, int col, double value, int index)
{
    nnz++;
    
    vals.insert(vals.begin() + index, value);
    rows.insert(rows.begin() + index, row);
    
    for (int i = col + 1; i < numCols + 1; i++)
        cols[i]++;
}

void gMatrixSparse::removeRandom(int col, int index)
{
    nnz--;
    
    vals.erase(vals.begin() + index);
    rows.erase(rows.begin() + index);
    
    for (int i = col + 1; i < numCols + 1; i++)
        cols[i]--;
}

#pragma mark COLUMN ACCESS

bool gMatrixSparse::colIsEmpty(int c)
{
    return cols[c] >= cols[c + 1];
}

gMatrixSparse::gColumnSparse gMatrixSparse::getColumn(int c)
{
    int idx = cols[c];
    return gColumnSparse(cols[c + 1] - idx, &vals[0] + idx, &rows[0] + idx);
}

#pragma mark SECRET CLASSES

#pragma mark MATRIX ITERATOR

gMatrixSparse::iterator::iterator(gMatrixSparse &mat)
: mat(mat), idx(-1), col(-1), row(-1), val(NAN)
{
    next();
}

bool gMatrixSparse::iterator::next() {
    if (++idx >= mat.nnz) return false;
    
    while (idx >= mat.cols[col + 1]) col++;
    row = mat.rows[idx];
    val = mat.vals[idx];
    
    return true;
}

#pragma mark MATRIX COLUMN

void gMatrixSparse::gColumnSparse::operator*=(double right)
{
    for (int i = 0; i < nnz; i++)
        vals[i] *= right;
}

double gMatrixSparse::gColumnSparse::dot(gColumnSparse &right)
{
    double sum = 0.0;
    iterator lit(*this), rit(right);
    
    while (lit.good() && rit.good()) {
        if (lit < rit) {
            lit.next();
        } else if (lit == rit) {
            sum += lit.val * rit.val;
            lit.next(); rit.next();
        } else {
            rit.next();
        }
    }
    
    return sum;
}

double gMatrixSparse::gColumnSparse::getNorm(gMatrixd::NormDef n)
{
    switch (n) {
        case gMatrixd::NORM_ONE:
        {
            double sum = 0.0;
            for (int i = 0; i < nnz; i++)
                sum += fabs(vals[i]);
            return sum;
        }
        case gMatrixd::NORM_TWO: default:
        {
            double sum = 0.0;
            double xmax = getNorm(gMatrixd::NORM_INF);
            for (int i = 0; i < nnz; i++)
                sum += (vals[i] * vals[i]) / (xmax * xmax);
            return xmax * sqrt(sum);
        }
        case gMatrixd::NORM_INF:
        {
            double max = 0.0;
            for (int i = 0; i < nnz; i++)
                if (fabs(vals[i]) > max) max = fabs(vals[i]);
            return max;
        }
    }
}

void gMatrixSparse::gColumnSparse::normalize(gMatrixd::NormDef n)
{
    (*this)*= 1.0 / getNorm(n);
}

#pragma mark MATRIX COLUMN ITERATOR

void gMatrixSparse::gColumnSparse::iterator::next()
{
    if (++idx >= col.nnz) return;
    val = col.vals[idx];
    row = col.rows[idx];
}

struct gColumnSparse {
private:
    int nnz, *rows;
    double *vals;
    
public:
    struct iterator {
        double val;
        int row, idx;
    private:
        gColumnSparse &col;
        
    public:
        iterator(gColumnSparse &aCol)
        : col(aCol), val(NAN), row(-1), idx(-1) { next(); }
        
        void next();
        
        inline bool good() { return idx < col.nnz; }
    };
    
public:
    gColumnSparse(int nnz, double *valptr, int *rowptr);
    
    void operator*=(double right);
    double dot(gColumnSparse &right);
    
    void normalize();
};

#pragma mark USEFUL CALLBACKS

bool elmSort(const gMatrixSparse::elm &one, const gMatrixSparse::elm &two)
{
    return coordSort(one.row, one.col, two.row, two.col);
}