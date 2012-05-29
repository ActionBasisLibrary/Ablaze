//
//  GInterpolatedValue2f.h
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

class gInterpolatedValue2f {
private:
    gVector2f start, end;
    gEasing::gEasingFunction function;
public:
    gInterpolatedValue2f(gEasing::gEasingFunction aFunction = gEasing::gLinear);
    gInterpolatedValue2f(gVector2f aStart, gVector2f aEnd, gEasing::gEasingFunction aFunction = gEasing::gLinear);
    
    void set(gVector2f aStart, gVector2f aEnd);
    void setFunction(gEasing::gEasingFunction aFunction);
    
    gVector2f getStart();
    gVector2f getEnd();
    
    gVector2f valueAt(gnum t);
};

#pragma GCC visibility pop