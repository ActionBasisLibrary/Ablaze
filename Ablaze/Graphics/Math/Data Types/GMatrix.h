//
//  GMatrix.h
//  GMath
//
//  Created by Caleb Jordan on 11/14/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include <assert.h>
#include <ostream>
#include <strings.h>

class gMatrixd {
public:
    typedef void (*mapFn)(const int row, const int col, double &value, const void *stuff);
    
    typedef double (*opFn)(double left, double right);
    
    enum NormDef {
        NORM_ONE, NORM_TWO, NORM_INF, NUM_TYPES
    };
    
public:
    gMatrixd() {}
    ~gMatrixd() {}
    
    virtual int getNumRows() = 0;
    virtual int getNumCols() = 0;
    virtual int getNumElements() = 0;
    
    virtual double at(int row, int col) = 0;
    virtual void set(int row, int col, double value) = 0;
    virtual void map(mapFn function, const void *stuff) = 0;
    
    void printMatrix(std::ostream &stream, std::string &format);
};