//
//  GMatrix3f.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/31/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GMatrix3f.h"

static const gnum IDENTITYVALS[9] = {
    1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0
};

gMatrix3f::gMatrix3f()
{
    memcpy(elements, IDENTITYVALS, 9 * sizeof(gnum));
}

gMatrix3f::gMatrix3f(gnum elems[9])
{
    memcpy(elements, elems, 9 * sizeof(gnum));
}

gMatrix3f::gMatrix3f(gVector3f rowOne, gVector3f rowTwo, gVector3f rowThree)
{
    for (unsigned int i = 0; i < 3; i++) {
        setVal(0, i, rowOne[i]);
        setVal(1, i, rowTwo[i]);
        setVal(2, i, rowThree[i]);
    }
}

gVector3f gMatrix3f::operator*(gVector3f right)
{
    return gVector3f(getRow(0).dot(right), getRow(1).dot(right), getRow(2).dot(right));
}

void gMatrix3f::operator+=(gMatrix3f &right)
{
    for (unsigned int i = 0; i < 9; i++) {
        getVal(i) += right.getVal(i);
    }
}

void gMatrix3f::operator-=(gMatrix3f &right)
{
    for (unsigned int i = 0; i < 9; i++) {
        getVal(i) -= right.getVal(i);
    }
}

void gMatrix3f::operator*=(gnum right)
{
    for (unsigned int i = 0; i < 9; i++) {
        getVal(i) *= right;
    }
}

gMatrix3f gMatrix3f::operator*(gMatrix3f &right)
{
    gMatrix3f result;
    
    for (unsigned int row = 0; row < 3; row++) {
        for (unsigned int col = 0; col < 3; col++) {
            result[row][col] = getRow(row).dot(right.getColumn(col));
        }
    }
    
    return result;
}

gMatrix3f gMatrix3f::operator+(gMatrix3f &right)
{
    gMatrix3f result(elements);
    
    result += right;
    
    return result;
}

gMatrix3f gMatrix3f::operator-(gMatrix3f &right)
{
    gMatrix3f result(elements);
    
    result -= right;
    
    return result;
}

gMatrix3f gMatrix3f::operator*(gnum right)
{
    gMatrix3f result(elements);
    
    result *= right;
    
    return result;
}

gnum gMatrix3f::getDeterminant()
{
    return (getVal(0)*(getVal(4)*getVal(8)-getVal(5)*getVal(7)) -
            getVal(1)*(getVal(3)*getVal(8)-getVal(5)*getVal(6)) +
            getVal(2)*(getVal(3)*getVal(7)-getVal(4)*getVal(6)));
}

gMatrix3f gMatrix3f::getTranspose()
{
    gMatrix3f result;
    
    for (unsigned int row = 0; row < 3; row++) {
        for (unsigned int col = 0; col < 3; col++) {
            result.setVal(col, row, getVal(row, col));
        }
    }
    
    return result;
}

gMatrix3f gMatrix3f::getInverse()
{
    gnum det = getDeterminant();
    
    if (det == 0.0) {
        fprintf(stderr, "Trying to invert a non-invertable matrix--returning it.\n");
        return *this;
    }
    
    gVector3f *rows = (gVector3f *)elements;
    gVector3f cpRows[3] = {
        rows[1].cross(rows[2]),
        rows[2].cross(rows[0]),
        rows[0].cross(rows[1])
    };
    
    gMatrix3f cfMatrix = gMatrix3f((gnum *)cpRows).getTranspose();
    
    cfMatrix *= 1.0 / det;
    
    return cfMatrix;
}

void gMatrix3f::getOpenGLSubMatrixf(float matrix[16])
{
    for (int i = 0; i < 3; i++) {
        matrix[i] = getVal(i, 0);
        matrix[4 + i] = getVal(i, 1);
        matrix[8 + i] = getVal(i, 2);
    }
}

void gMatrix3f::setFromOpenGLSubMatrixf(float matrix[16])
{
    for (int i = 0; i < 3; i++) {
        setVal(i, 0, matrix[i]);
        setVal(i, 1, matrix[4 + i]);
        setVal(i, 2, matrix[8 + i]);
    }
}

#pragma mark STATIC FUNCTIONS

gMatrix3f gMatrix3f::getRotation(gnum rads, gVector3f axis)
{
    axis.normalize();
    
    gnum x = axis.x, y = axis.y, z = axis.z;
    gnum xx = x*x, xy = x*y, xz = x*z, yy = y*y, yz = y*z, zz = z*z;
    gnum c = cosf(rads), s = sinf(rads), ic = 1 - c;
    
    gnum elems[] = {
        c + xx*ic, xy*ic - z*s, xz*ic + y*s,
        xy*ic + z*s, c + yy*ic, yz*ic - x*s,
        xz*ic - y*s, yz*ic + x*s, c + zz*ic
    };
    
    gMatrix3f result(elems);
    
    return result;
}
