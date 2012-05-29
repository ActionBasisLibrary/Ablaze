//
//  GEasing.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GEasing.h"

gnum gEasing::gSwitch(gnum t)
{
    return t < 0.5 ? 0.0 : 1.0;
}

gnum gEasing::gLinear(gnum t)
{
    return fmin(fmax(t, 0.0), 1.0);
}

gnum gEasing::gCubic(gnum t)
{
    t = gLinear(t);
    return -6.0 * t * (t * t / 3.0 - t * 0.5);
}

gnum gEasing::gCosine(gnum t)
{
    return 0.5 * gcos(M_PI * gLinear(t)) + 0.5;
}

gnum gEasing::gCubicInOut(gnum t)
{
    t = gLinear(t);
    return t * (t * (6.75 * t - 13.5) + 6.75);
}