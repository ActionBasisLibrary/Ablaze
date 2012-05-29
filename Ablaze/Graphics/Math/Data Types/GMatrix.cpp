//
//  GMatrix.cpp
//  GMath
//
//  Created by Caleb Jordan on 11/14/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GMatrix.h"

#pragma mark PRINT

static struct printStruct {
    std::ostream &stream;
    std::string &format;
    printStruct(std::ostream &str, std::string fmt)
    : stream(str), format(fmt) {}
};

void printCallback(const int row, const int col, double &value, const void *stuff);

void gMatrixd::printMatrix(std::ostream &stream, std::string &format)
{
    printStruct stuff (stream, format);
    
    stream << "Matrix [" << getNumRows() << ", " << getNumCols() << "], (" << getNumElements() << "):";
    map(printCallback, &stuff);
    stream << std::endl;
}

void printCallback(const int row, const int col, double &value, const void *stuff)
{
    printStruct *pStuff = (printStruct *)stuff;
    
    pStuff->stream << pStuff->format << "(" << row << ", " << col << "): " << value;
}