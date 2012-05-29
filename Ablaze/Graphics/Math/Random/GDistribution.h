//
//  GDistribution.h
//  GMath
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include <math.h>

#include "GRand.h"
#include "GDataTypes.h"

#pragma GCC visibility push(default)

namespace gDistribution {
    
    typedef gVector3f (*gDistribution3f)(void);
    typedef gVector2f (*gDistribution2f)(void);
    typedef gnum (*gDistribution1f)(void);
    
    gnum gDefault();
    
    gVector3f gCube();
    gVector3f gSphere();
    gVector3f gShell();
    
    gVector2f gSquare();
    gVector2f gDisk();
    gVector2f gCircle();
}

#pragma GCC visibility pop