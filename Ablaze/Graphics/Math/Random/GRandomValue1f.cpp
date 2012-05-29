//
//  GRandomValue1f.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GRandomValue1f.h"

gRandomValue1f::gRandomValue1f(gDistribution::gDistribution1f aFunction)
: min(0.0), max(1.0), function(aFunction)
{
    srand((unsigned int)clock());
}

gRandomValue1f::gRandomValue1f(gnum aMin, gnum aMax, gDistribution::gDistribution1f aFunction)
: min(aMin), max(aMax), function(aFunction)
{
    srand((unsigned int)clock());
}

void gRandomValue1f::set(gnum aMin, gnum aMax)
{
    min = aMin;
    max = aMax;
}

void gRandomValue1f::setFunction(gDistribution::gDistribution1f aFunction)
{
    function = aFunction;
}

gnum gRandomValue1f::getMin()
{
    return min;
}

gnum gRandomValue1f::getMax()
{
    return max;
}

gnum gRandomValue1f::value()
{
    return function() * (max - min) + min;
}