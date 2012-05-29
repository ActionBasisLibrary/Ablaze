//
//  GInterpolatedValue1f.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GInterpolatedValue1f.h"

gInterpolatedValue1f::gInterpolatedValue1f(gEasing::gEasingFunction aFunction)
: start(0.0), end(1.0), function(aFunction)
{
    
}

gInterpolatedValue1f::gInterpolatedValue1f(gnum aStart, gnum aEnd, gEasing::gEasingFunction aFunction)
: start(aStart), end(aEnd), function(aFunction)
{
    
}

void gInterpolatedValue1f::set(gnum aStart, gnum aEnd)
{
    start = aStart;
    end = aEnd;
}

void gInterpolatedValue1f::setFunction(gEasing::gEasingFunction aFunction)
{
    function = aFunction;
}

gnum gInterpolatedValue1f::getStart()
{
    return start;
}

gnum gInterpolatedValue1f::getEnd()
{
    return end;
}

gnum gInterpolatedValue1f::valueAt(gnum t)
{
    return (end - start) * function(t) + start;
}