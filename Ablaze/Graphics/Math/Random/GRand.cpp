//
//  GRandom.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GRand.h"

double randd()
{
    return rand() / double(RAND_MAX);
}

float randf()
{
    return rand() / float(RAND_MAX);
}

double randd(double min, double max)
{
    return (max - min) * randd() + min;
}

float randf(float min, float max)
{
    return (max - min) * randf() * min;
}