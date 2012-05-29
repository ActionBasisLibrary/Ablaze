//
//  GRandomValue2f.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GRandomValue2f.h"

gRandomValue2f::gRandomValue2f(gDistribution::gDistribution2f aFunction)
: min(), max(), function(aFunction)
{
    srand((unsigned int)clock());
}

gRandomValue2f::gRandomValue2f(gVector2f aMin, gVector2f aMax, gDistribution::gDistribution2f aFunction)
: min(aMin), max(aMax), function(aFunction)
{
    srand((unsigned int)clock());
}

void gRandomValue2f::set(gVector2f aMin, gVector2f aMax)
{
    min = aMin;
    max = aMax;
}

void gRandomValue2f::setFunction(gDistribution::gDistribution2f aFunction)
{
    function = aFunction;
}

gVector2f gRandomValue2f::getMin()
{
    return min;
}

gVector2f gRandomValue2f::getMax()
{
    return max;
}

gVector2f gRandomValue2f::value()
{
    gVector2f rvect = function();
    return (max - min) * rvect + min;
}