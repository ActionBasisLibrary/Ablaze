//
//  GRandomValue1f.h
//  GMath
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include "GTypes.h"
#include "GRand.h"
#include "GDistribution.h"

#pragma GCC visibility push(default)

class gRandomValue1f {
private:
    gnum min, max;
    gDistribution::gDistribution1f function;
public:
    gRandomValue1f(gDistribution::gDistribution1f aFunction = gDistribution::gDefault);
    gRandomValue1f(gnum aMin, gnum aMax, gDistribution::gDistribution1f aFunction = gDistribution::gDefault);
    
    void set(gnum aMin, gnum aMax);
    void setFunction(gDistribution::gDistribution1f aFunction);
    
    gnum getMin();
    gnum getMax();
    
    gnum value();
};

#pragma GCC visibility pop