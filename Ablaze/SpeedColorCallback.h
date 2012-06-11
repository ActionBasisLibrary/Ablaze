//
//  SpeedColorCallback.h
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef Ablaze_SpeedColorCallback_h
#define Ablaze_SpeedColorCallback_h

#include "ABParticles.h"
#import "ABLWrapper.h"

class SpeedColorCallback : public ABParticles::ProfileCallback {
public:
	ABLWrapper *wrapper;
	
	void startPosition (gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	void startVelocity (gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	void startColor    (gVector4f &vect, float dt, const ABParticles::Particle *ptr);
	void acceleration  (gVector3f &vect, float dt, const ABParticles::Particle *ptr);
};

#endif
