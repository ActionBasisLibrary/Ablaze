//
//  GInterpolatedValue2f.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GInterpolatedValue2f.h"

gInterpolatedValue2f::gInterpolatedValue2f(gEasing::gEasingFunction aFunction)
: start(), end(1.0, 1.0), function(aFunction)
{
    
}

gInterpolatedValue2f::gInterpolatedValue2f(gVector2f aStart, gVector2f aEnd, gEasing::gEasingFunction aFunction)
: start(aStart), end(aEnd), function(aFunction)
{
    
}

void gInterpolatedValue2f::set(gVector2f aStart, gVector2f aEnd)
{
    start = aStart;
    end = aEnd;
}

void gInterpolatedValue2f::setFunction(gEasing::gEasingFunction aFunction)
{
    function = aFunction;
}

gVector2f gInterpolatedValue2f::getStart()
{
    return start;
}

gVector2f gInterpolatedValue2f::getEnd()
{
    return end;
}

gVector2f gInterpolatedValue2f::valueAt(gnum t)
{
    return (end - start) * function(t) + start;
}