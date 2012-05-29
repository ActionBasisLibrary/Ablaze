//
//  GVector2f.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GVector2f.h"

gVector2f::gVector2f()
: x(0.0), y(0.0)
{
    
}

gVector2f::gVector2f(gnum all)
{
    x = y = all;
}

gVector2f::gVector2f(gnum x, gnum y)
: x(x), y(y)
{ }

void gVector2f::set(gnum x, gnum y)
{
    this->x = x;
    this->y = y;
}

void gVector2f::operator+=(gVector2f right)
{
    x += right.x;
    y += right.y;
}

void gVector2f::operator*=(gnum right)
{
    x *= right;
    y *= right;
}

gVector2f gVector2f::operator+(gVector2f right)
{
    return gVector2f(x + right.x, y + right.y);
}

gVector2f gVector2f::operator-(gVector2f right)
{
    return gVector2f(x - right.x, y - right.y);
}

gVector2f gVector2f::operator*(gVector2f right)
{
    return gVector2f(x * right.x, y * right.y);
}

gVector2f gVector2f::operator*(gnum right)
{
    return gVector2f(x * right, y * right);
}

gnum gVector2f::dot(gVector2f right)
{
    return x * right.x + y * right.y;
}

gnum gVector2f::lengthSq()
{
    return x * x + y * y;
}

gnum gVector2f::length()
{
    return gsqrt(lengthSq());
}

void gVector2f::normalize()
{
    operator*=(1.0 / length());
}

#pragma mark STATIC STUFF

gVector2f gVector2f::min(gVector2f left, gVector2f right)
{
    return gVector2f(gmin(left.x, right.x), gmin(left.y, right.y));
}

gVector2f gVector2f::max(gVector2f left, gVector2f right)
{
    return gVector2f(gmax(left.x, right.x), gmax(left.y, right.y));
}