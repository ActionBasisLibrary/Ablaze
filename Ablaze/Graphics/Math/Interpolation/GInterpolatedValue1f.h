//
//  GInterpolatedValue1f.h
//  GMath
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include "GTypes.h"
#include "GEasing.h"

#pragma GCC visibility push(default)

class gInterpolatedValue1f {
private:
    gnum start, end;
    gEasing::gEasingFunction function;
public:
    gInterpolatedValue1f(gEasing::gEasingFunction aFunction = gEasing::gLinear);
    gInterpolatedValue1f(gnum aStart, gnum aEnd, gEasing::gEasingFunction aFunction = gEasing::gLinear);
    
    void set(gnum aStart, gnum aEnd);
    void setFunction(gEasing::gEasingFunction aFunction);
    
    gnum getStart();
    gnum getEnd();
    
    gnum valueAt(gnum t);
};

#pragma GCC visibility pop