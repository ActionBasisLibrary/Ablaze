//
//  GVector3f.cpp
//  GParticles
//
//  Created by Caleb Jordan on 8/10/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GVector3f.h"

gVector3f::gVector3f()
: x(0.0), y(0.0), z(0.0)
{
    
}

gVector3f::gVector3f(gnum all)
{
    x = y = z = all;
}

gVector3f::gVector3f(gnum x, gnum y, gnum z)
: x(x), y(y), z(z)
{ }

void gVector3f::set(gnum x, gnum y, gnum z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void gVector3f::operator+=(gVector3f right)
{
    x += right.x;
    y += right.y;
    z += right.z;
}

void gVector3f::operator*=(gnum right)
{
    x *= right;
    y *= right;
    z *= right;
}

gVector3f gVector3f::operator+(gVector3f right)
{
    return gVector3f(x + right.x, y + right.y, z + right.z);
}

gVector3f gVector3f::operator-(gVector3f right)
{
    return gVector3f(x - right.x, y - right.y, z - right.z);
}

gVector3f gVector3f::operator*(gVector3f right)
{
    return gVector3f(x * right.x, y * right.y, z * right.z);
}

gVector3f gVector3f::operator*(gnum right)
{
    return gVector3f(x * right, y * right, z * right);
}

gnum gVector3f::dot(gVector3f right)
{
    return x * right.x + y * right.y + z * right.z;
}

gnum gVector3f::lengthSq()
{
    return x * x + y * y + z * z;
}

gnum gVector3f::length()
{
    return gsqrt(lengthSq());
}

gVector3f gVector3f::cross(gVector3f right)
{
    return gVector3f(y*right.z - z*right.y,
                     z*right.x - x*right.z,
                     x*right.y - y*right.x);
}

void gVector3f::normalize()
{
    operator*=(1.0 / length());
}

#pragma mark STATIC STUFF

gVector3f gVector3f::min(gVector3f left, gVector3f right)
{
    return gVector3f(gmin(left.x, right.x), gmin(left.y, right.y), gmin(left.z, right.z));
}

gVector3f gVector3f::max(gVector3f left, gVector3f right)
{
    return gVector3f(gmax(left.x, right.x), gmax(left.y, right.y), gmax(left.z, right.z));
}