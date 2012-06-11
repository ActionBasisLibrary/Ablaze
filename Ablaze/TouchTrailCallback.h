//
//  TouchTrailCallback.h
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef Ablaze_TouchTrailCallback_h
#define Ablaze_TouchTrailCallback_h

#include "ABParticles.h"

class TouchTrailCallback : public ABParticles::ProfileCallback {
	void startPosition(gVector3f &vect, float dt, const ABParticles::Particle *ptr);
};


#endif
