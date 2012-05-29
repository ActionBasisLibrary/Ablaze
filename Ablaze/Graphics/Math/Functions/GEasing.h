//
//  GEasing.h
//  GMath
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include <math.h>

#include "GTypes.h"

#pragma GCC visibility push(default)

namespace gEasing {
    
    typedef gnum (*gEasingFunction)(gnum t);
    
    gnum gSwitch(gnum t);
    gnum gLinear(gnum t);
    gnum gCubic(gnum t);
    gnum gCosine(gnum t);
    
    gnum gCubicInOut(gnum t);
}

#pragma GCC visibility pop