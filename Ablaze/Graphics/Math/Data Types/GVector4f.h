//
//  GVector4f.h
//  GMath
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include <string.h>

#include "GTypes.h"
#include "GVector3f.h"

#pragma GCC visibility push(default)

struct gVector4f {
    gnum r, g, b, a;
    
    gVector4f();
    gVector4f(gnum r, gnum g, gnum b, gnum a = 1.0);
    gVector4f(gVector3f rgb, gnum a = 1.0);
    void set(gnum r, gnum g, gnum b, gnum a = 1.0);
    void set(gVector3f rgb, gnum a = 1.0);
    
    inline gnum &operator[](unsigned int idx) { return (&r)[idx]; }
    
    void operator+=(gVector4f right);
    void operator*=(gnum right);
    
    gVector4f operator+(gVector4f right);
    gVector4f operator-(gVector4f right);
    gVector4f operator*(gVector4f right);
    
    gVector4f operator*(gnum right);
    
    gnum dot(gVector4f right);
    gnum lengthSq();
    gnum length();
    
    void normalize();
    
    static gVector4f min(gVector4f left, gVector4f right);
    static gVector4f max(gVector4f left, gVector4f right);
};

#pragma GCC visibility pop