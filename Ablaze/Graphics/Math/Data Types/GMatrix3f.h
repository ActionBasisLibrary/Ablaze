//
//  GMatrix3f.h
//  GMath
//
//  Created by Caleb Jordan on 8/31/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <memory.h>

#include "GTypes.h"
#include "GVector3f.h"

#pragma GCC visibility push(default)

class gMatrix3f {
private:
    gnum elements[9];
    
public:
    gMatrix3f();
    gMatrix3f(gnum elems[9]);
    gMatrix3f(gVector3f rowOne, gVector3f rowTwo, gVector3f rowThree);
    
    inline gVector3f getColumn(unsigned int idx) { return gVector3f(getVal(0, idx), getVal(1, idx), getVal(2, idx)); }
    inline gVector3f getRow(unsigned int idx) { return gVector3f(getVal(idx, 0), getVal(idx, 1), getVal(idx, 2)); }
    
    inline gnum getVal(unsigned int row, unsigned int col) { return elements[row * 3 + col]; }
    inline void setVal(unsigned int row, unsigned int col, gnum value) { elements[row * 3 + col] = value; }
    
    inline gnum *operator[](unsigned int idx) { return elements + 3 * idx; }
    
    gVector3f operator*(gVector3f right);
    
    void operator+=(gMatrix3f &right);
    void operator-=(gMatrix3f &right);
    void operator*=(gnum right);
    
    gMatrix3f operator*(gMatrix3f &right);
    gMatrix3f operator+(gMatrix3f &right);
    gMatrix3f operator-(gMatrix3f &right);
    gMatrix3f operator*(gnum right);
    
    gnum getDeterminant();
    gMatrix3f getTranspose();
    gMatrix3f getInverse();

    void getOpenGLSubMatrixf(float matrix[16]);
    void setFromOpenGLSubMatrixf(float matrix[16]);
    
    static gMatrix3f getRotation(double rads, gVector3f axis);
    
private:
    inline gnum &getVal(unsigned int idx) { return elements[idx]; }
};

#pragma GCC visibility pop