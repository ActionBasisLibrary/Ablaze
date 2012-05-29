//
//  GRandomValue3f.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GRandomValue3f.h"

gRandomValue3f::gRandomValue3f(gDistribution::gDistribution3f aFunction)
: min(), max(), function(aFunction)
{
    srand((unsigned int)clock());
}

gRandomValue3f::gRandomValue3f(gVector3f aMin, gVector3f aMax, gDistribution::gDistribution3f aFunction)
: min(aMin), max(aMax), function(aFunction)
{
    srand((unsigned int)clock());
}

void gRandomValue3f::set(gVector3f aMin, gVector3f aMax)
{
    min = aMin;
    max = aMax;
}

void gRandomValue3f::setFunction(gDistribution::gDistribution3f aFunction)
{
    function = aFunction;
}


gVector3f gRandomValue3f::getMin()
{
    return min;
}

gVector3f gRandomValue3f::getMax()
{
    return max;
}

gVector3f gRandomValue3f::value()
{
    gVector3f rvect = function();
    return (max - min) * rvect + min;
}