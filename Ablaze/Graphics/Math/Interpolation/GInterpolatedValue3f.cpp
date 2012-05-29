//
//  GInterpolatedValue3f.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GInterpolatedValue3f.h"

gInterpolatedValue3f::gInterpolatedValue3f(gEasing::gEasingFunction aFunction)
: start(), end(1.0, 1.0, 1.0), function(aFunction)
{
    
}

gInterpolatedValue3f::gInterpolatedValue3f(gVector3f aStart, gVector3f aEnd, gEasing::gEasingFunction aFunction)
: start(aStart), end(aEnd), function(aFunction)
{
    
}

void gInterpolatedValue3f::set(gVector3f aStart, gVector3f aEnd)
{
    start = aStart;
    end = aEnd;
}

void gInterpolatedValue3f::setFunction(gEasing::gEasingFunction aFunction)
{
    function = aFunction;
}

gVector3f gInterpolatedValue3f::getStart()
{
    return start;
}

gVector3f gInterpolatedValue3f::getEnd()
{
    return end;
}

gVector3f gInterpolatedValue3f::valueAt(gnum t)
{
    return (end - start) * function(t) + start;
}