//
//  GInterpolatedValue3f.h
//  GMath
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include "GTypes.h"
#include "GDataTypes.h"
#include "GEasing.h"

#pragma GCC visibility push(default)

class gInterpolatedValue3f {
private:
    gVector3f start, end;
    gEasing::gEasingFunction function;
public:
    gInterpolatedValue3f(gEasing::gEasingFunction aFunction = gEasing::gLinear);
    gInterpolatedValue3f(gVector3f aStart, gVector3f aEnd, gEasing::gEasingFunction aFunction = gEasing::gLinear);
    
    void set(gVector3f aStart, gVector3f aEnd);
    void setFunction(gEasing::gEasingFunction aFunction);
    
    gVector3f getStart();
    gVector3f getEnd();
    
    gVector3f valueAt(gnum t);
};

#pragma GCC visibility pop