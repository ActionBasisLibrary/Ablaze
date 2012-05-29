//
//  GRandomValue3f.h
//  GMath
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include "GRand.h"
#include "GDataTypes.h"
#include "GDistribution.h"

#pragma GCC visibility push(default)

class gRandomValue3f {
private:
    gVector3f min, max;
    gDistribution::gDistribution3f function;
public:
    gRandomValue3f(gDistribution::gDistribution3f aFunction = gDistribution::gCube);
    gRandomValue3f(gVector3f aMin, gVector3f aMax, gDistribution::gDistribution3f aFunction = gDistribution::gCube);
    
    void set(gVector3f aMin, gVector3f aMax);
    void setFunction(gDistribution::gDistribution3f aFunction);
    
    gVector3f getMin();
    gVector3f getMax();
    
    gVector3f value();
};

#pragma GCC visibility pop