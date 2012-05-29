//
//  GVector3f.h
//  GParticles
//
//  Created by Caleb Jordan on 8/10/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include "GTypes.h"
#include "GVector2f.h"

#pragma GCC visibility push(default)

struct gVector3f {
    gnum x, y, z;
    
    gVector3f();
    gVector3f(gnum all);
    gVector3f(gnum x, gnum y, gnum z);
    void set(gnum x, gnum y, gnum z);
    
    inline gVector2f xy() { return gVector2f(x, y); }
    
    inline gnum &operator[](unsigned int idx) { return (&x)[idx]; }
    
    void operator+=(gVector3f right);
    void operator*=(gnum right);
    
    gVector3f operator+(gVector3f right);
    gVector3f operator-(gVector3f right);
    gVector3f operator*(gVector3f right);
    
    gVector3f operator*(gnum right);
    
    gnum dot(gVector3f right);
    gnum lengthSq();
    gnum length();
    
    gVector3f cross(gVector3f right);
    
    void normalize();
    
    static gVector3f min(gVector3f left, gVector3f right);
    static gVector3f max(gVector3f left, gVector3f right);
};

#pragma GCC visibility pop