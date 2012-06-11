//
//  KaleidoscopeCallback.h
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef Ablaze_KaleidoscopeCallback_h
#define Ablaze_KaleidoscopeCallback_h

#include "ABParticles.h"
#import "KaleidoscopeTransform.h"

class KaleidoscopeCallback : public ABParticles::ProfileCallback {
public:
	KaleidoscopeTransform *transform;
	
	void startSize(float *size, float dt, const ABParticles::Particle *ptr);
	void size(float *size, float dt, const ABParticles::Particle *ptr);
	void startColor(gVector4f &color, float dt, const ABParticles::Particle *ptr);
	void startPosition(gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	//void position(gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	void startVelocity(gVector3f &color, float dt, const ABParticles::Particle *ptr);
	//void velocity(gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	void acceleration(gVector3f &vect, float dt, const ABParticles::Particle *ptr);
};

#endif
