//
//  GRigidTransform.h
//  GMath
//
//  Created by Caleb Jordan on 9/1/11.
//  Copyright 2011 Stanford University. All rights reserved.
//


#pragma once

#include <stdio.h>
#include <memory.h>

#include "GTypes.h"
#include "GVector3f.h"
#include "GMatrix3f.h"

#pragma GCC visibility push(default)

class gRigidTransform {
private:
    gMatrix3f rotationBasis;
    gVector3f origin;
    
public:
    gRigidTransform();
    gRigidTransform(gMatrix3f rotation, gVector3f origin);
    //gRigidTransform(gMatrix3f &rotation, gVector3f &origin);
    
    void setRotation(gMatrix3f &rotation);
    void setTranslation(gVector3f &translation);
    
    inline gMatrix3f getRotation() { return rotationBasis; }
    inline gVector3f getTranslation() { return origin; }
    
    void rotateBy(gMatrix3f &rotation);
    void translateBy(gVector3f &translation);
    
    void operator*=(gRigidTransform &other);
    gRigidTransform operator*(gRigidTransform other);
    
    gRigidTransform getInverse();
    
    void getOpenGLMatrixf(float matrix[16]);
    void setFromOpenGLMatrixf(float matrix[16]);
};

#pragma GCC visibility pop
