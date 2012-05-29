//
//  GVector2f.h
//  GMath
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//


#pragma once

#include "GTypes.h"

#pragma GCC visibility push(default)

struct gVector2f {
    gnum x, y;
    
    gVector2f();
    gVector2f(gnum all);
    gVector2f(gnum x, gnum y);
    void set(gnum x, gnum y);
    
    inline gnum &operator[](unsigned int idx) { return (&x)[idx]; }
    
    void operator+=(gVector2f right);
    void operator*=(gnum right);
    
    gVector2f operator+(gVector2f right);
    gVector2f operator-(gVector2f right);
    gVector2f operator*(gVector2f right);
    
    gVector2f operator*(gnum right);
    
    gnum dot(gVector2f right);
    gnum lengthSq();
    gnum length();
    
    void normalize();
    
    static gVector2f min(gVector2f left, gVector2f right);
    static gVector2f max(gVector2f left, gVector2f right);
};

#pragma GCC visibility pop