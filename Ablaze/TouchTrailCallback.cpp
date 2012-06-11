//
//  TouchTrailCallback.cpp
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "TouchTrailCallback.h"


void TouchTrailCallback::startPosition(gVector3f &position, float dt, const ABParticles::Particle *ptr)
{
	position = gVector3f(100.0f, 100.0f, 0.0f);
}
