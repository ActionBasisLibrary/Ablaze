//
//  GMatrixSparse.h
//  GMath
//
//  Created by Caleb Jordan on 11/14/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include "GMatrix.h"

#include <vector>
#include <map>
#include <math.h>

using namespace std;

class gMatrixSparse : public gMatrixd {
public:
    struct elm {
        double val;
        int row, col;
        elm(int r, int c, double v)
        : val(v), row(r), col(c) {}
    };
    
private:
    // Iterator
    struct iterator {
    private:
        const gMatrixSparse &mat;
        
    public:
        int idx, col, row, val;
        
        iterator(gMatrixSparse &mat);
        
        bool next();
        
        inline bool good() { return idx < mat.nnz; }
        
        inline bool operator==(iterator &other)
        { return row == other.row && col == other.col; }
        
        inline bool operator<(iterator &other)
        { return col < other.col && row < other.row; }
    };
    
    // Column
    struct gColumnSparse {
    private:
        int nnz, *rows;
        double *vals;
        
    public:
        inline gColumnSparse(int nnz, double *valptr, int *rowptr)
        : nnz(nnz), vals(valptr), rows(rowptr) {}
        inline gColumnSparse()
        : nnz(0), vals(NULL), rows(NULL) {}
        
        void operator*=(double right);
        double dot(gColumnSparse &right);
        double getNorm(NormDef n = NORM_TWO);
        void normalize(NormDef n = NORM_TWO);
        
        // Iterator
        struct iterator {
        private:
            gColumnSparse &col;
            
        public:
            double val;
            int row, idx;
            
            inline iterator(gColumnSparse &aCol)
            : col(aCol), val(NAN), row(-1), idx(-1) { next(); }
            inline bool good() { return idx < col.nnz; }
            inline bool operator<(iterator &right) { return row < right.row; }
            inline bool operator==(iterator &right) { return row == right.row; }
            
            void next();
        };
    };
    
    struct dualMapper {
        virtual void map(int row, int col, double valLeft, double valRight) = 0;
    };
    
    int nnz, numRows, numCols;
    vector<double> vals;
    vector<int> rows;
    vector<int> cols;
    
    double eps;
    
public:
    // Constructors
    gMatrixSparse(int rows, int cols);
    gMatrixSparse(int rows, int cols, int nnz);
    gMatrixSparse(int rows, int cols, vector<elm> &elements, bool sorted = false);
    ~gMatrixSparse();
    
    // Easy Shit
    virtual int getNumRows() { return numRows; }
    virtual int getNumCols() { return numCols; }
    virtual int getNumElements() { return nnz; }
    
    inline double getEps() { return eps; }
    inline void setEps(double e) { eps = e; }
    
    // Random Access
    virtual double at(int row, int col);
    virtual void set(int row, int col, double value);
    virtual void map(mapFn function, const void *stuff);
    
    // Operators
    gMatrixSparse operator+(gMatrixSparse &right);
    gMatrixSparse operator-(gMatrixSparse &right);
    gMatrixSparse operator*(gMatrixSparse &right);
    gMatrixSparse operator*(double scalar);
    
    void operator*=(double scalar);
    
    // Clever stuff one
    gMatrixSparse getTranspose();
    gMatrixSparse getNearSymmetric();
    
    bool isSymmetric();
    
    // Premade stuff
    static gMatrixSparse getIdentity(int numRows);
    static gMatrixSparse getIdentity(int numRows, int numCols);
    static gMatrixSparse eVect(int index, int length, bool transpose = false);
    
private:
    static void dualMap(gMatrixSparse &left, gMatrixSparse &right, dualMapper &mapper);
    
    void init();
    
    // Clever access helpers
    void allocate(int numnz);
    void fill(vector<elm> &vect, bool sorted = false);
    
    // Random Access Helpers
    bool getIndex(int row, int col, int &index);
    void insertRandom(int row, int col, double value, int index);
    void removeRandom(int col, int index);
    
    // Column Access
    bool colIsEmpty(int c);
    gColumnSparse getColumn(int c);
};