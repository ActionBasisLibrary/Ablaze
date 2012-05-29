//
//  GRandomValue2f.h
//  GMath
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include "GRand.h"
#include "GDataTypes.h"
#include "GDistribution.h"

#pragma GCC visibility push(default)

class gRandomValue2f {
private:
    gVector2f min, max;
    gDistribution::gDistribution2f function;
public:
    gRandomValue2f(gDistribution::gDistribution2f aFunction = gDistribution::gSquare);
    gRandomValue2f(gVector2f aMin, gVector2f aMax, gDistribution::gDistribution2f aFunction = gDistribution::gSquare);
    
    void set(gVector2f aMin, gVector2f aMax);
    void setFunction(gDistribution::gDistribution2f aFunction);
    
    gVector2f getMin();
    gVector2f getMax();
    
    gVector2f value();
};

#pragma GCC visibility pop