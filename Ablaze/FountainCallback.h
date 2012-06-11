//
//  FountainCallback.h
//  Ablaze
//
//  Created by Caleb Jordan on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "ABParticles.h"
#include "ABL/ABTransform.h"

class FountainCallback : public ABParticles::ProfileCallback {
public:
	ABTransform *transform;
    GTimer *timer;
    gVector2f center;
	
	void startPosition (gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	void startVelocity (gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	void acceleration  (gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	void color (gVector4f &vect, float dt, const ABParticles::Particle *ptr);
    void startSize (float *val, float dt, const ABParticles::Particle *ptr);
    void initOverride   (ABParticles::Particle *ptr);
};
