//
//  GRigidTransform.cpp
//  GMath
//
//  Created by Caleb Jordan on 9/1/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GRigidTransform.h"

gRigidTransform::gRigidTransform()
: rotationBasis(), origin(0.0)
{
    
}

gRigidTransform::gRigidTransform(gMatrix3f rotation, gVector3f origin)
: rotationBasis(rotation), origin(origin)
{
    
}
/*
gRigidTransform::gRigidTransform(gMatrix3f &rotation, gVector3f &origin)
: rotationBasis(rotation), origin(origin)
{
    
}*/

#pragma mark DATA

void gRigidTransform::setRotation(gMatrix3f &rotation)
{
    rotationBasis = rotation;
}

void gRigidTransform::setTranslation(gVector3f &translation)
{
    this->origin = translation;
}

void gRigidTransform::rotateBy(gMatrix3f &rotation)
{
    rotationBasis = rotationBasis * rotation;
}

void gRigidTransform::translateBy(gVector3f &translation)
{
    origin += translation;
}

#pragma mark USE IT

void gRigidTransform::operator*=(gRigidTransform &other)
{
    origin += rotationBasis * other.origin;
    rotationBasis = rotationBasis * other.rotationBasis;
}

gRigidTransform gRigidTransform::operator*(gRigidTransform other)
{
    gRigidTransform result = *this;
    result *= other;
    return result;
}

gRigidTransform gRigidTransform::getInverse()
{
    gMatrix3f rotInverse = rotationBasis.getTranspose();
    gRigidTransform result(rotInverse, rotInverse * origin * -1.0);
    return result;
}

void gRigidTransform::getOpenGLMatrixf(float matrix[16])
{
    matrix[3] = matrix[7] = matrix[11] = 0.0;
    rotationBasis.getOpenGLSubMatrixf(matrix);
    
    matrix[12] = origin.x;
    matrix[13] = origin.y;
    matrix[14] = origin.z;
    matrix[15] = 1.0;
}

void gRigidTransform::setFromOpenGLMatrixf(float matrix[16])
{
    rotationBasis.setFromOpenGLSubMatrixf(matrix);
    
    origin.x = matrix[12];
    origin.y = matrix[13];
    origin.z = matrix[14];
}