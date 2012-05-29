//
//  GVector4f.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GVector4f.h"

gVector4f::gVector4f()
: r(0.0), g(0.0), b(0.0), a(1.0)
{ }

gVector4f::gVector4f(gnum r, gnum g, gnum b, gnum a)
: r(r), g(g), b(b), a(a)
{ }

gVector4f::gVector4f(gVector3f rgb, gnum a)
: r(rgb.x), g(rgb.y), b(rgb.z), a(a)
{ }

void gVector4f::set(gnum r, gnum g, gnum b, gnum a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void gVector4f::set(gVector3f rgb, gnum a)
{
    memcpy(this, &rgb, sizeof(gVector3f));
    this->a = a;
}

void gVector4f::operator+=(gVector4f right)
{
    r += right.r;
    g += right.g;
    b += right.b;
    a += right.a;
}

void gVector4f::operator*=(gnum right)
{
    r *= right;
    g *= right;
    b *= right;
    a *= right;
}

gVector4f gVector4f::operator+(gVector4f right)
{
    return gVector4f(r + right.r, g + right.g, b + right.b);
}

gVector4f gVector4f::operator-(gVector4f right)
{
    return gVector4f(r - right.r, g - right.g, b - right.b);
}

gVector4f gVector4f::operator*(gVector4f right)
{
    return gVector4f(r * right.r, g * right.g, b * right.b);
}

gVector4f gVector4f::operator*(gnum right)
{
    return gVector4f(r * right, g * right, b * right);
}

gnum gVector4f::dot(gVector4f right)
{
    return r * right.r + g * right.g + b * right.b + a * right.a;
}

gnum gVector4f::lengthSq()
{
    return r * r + g * g + b * b + a * a;
}

gnum gVector4f::length()
{
    return gsqrt(lengthSq());
}

void gVector4f::normalize()
{
    operator*=(1.0 / length());
}

#pragma mark STATIC STUFF


gVector4f gVector4f::min(gVector4f left, gVector4f right)
{
    return gVector4f(gmin(left.r, right.r), gmin(left.g, right.g), gmin(left.b, right.b), gmin(left.a, right.a));
}

gVector4f gVector4f::max(gVector4f left, gVector4f right)
{
    return gVector4f(gmax(left.r, right.r), gmax(left.g, right.g), gmax(left.b, right.b), gmax(left.a, right.a));

}