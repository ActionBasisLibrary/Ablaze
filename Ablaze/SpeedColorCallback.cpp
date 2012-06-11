//
//  SpeedColorCallback.cpp
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "SpeedColorCallback.h"

#include "GRand.h"


void SpeedColorCallback::startPosition(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
	vect = particleStartPosition;
}

void SpeedColorCallback::startVelocity(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
	const float speed = (1.0-2.0*randf())*100.0;
	double angle = (2.0*M_PI*randf());
	vect.x = sin(angle)*speed;
	vect.y = cos(angle)*speed;
	vect.z = 0;
}

void SpeedColorCallback::startColor(gVector4f &vect, float dt, const ABParticles::Particle *ptr)
{
	vect = particleStartColor;
}

void SpeedColorCallback::acceleration(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
    double v2 = currVelocity.length() * .001;
    double drag = 1.5 * exp(-pow(v2 + .5, -4.0)) - 1.0;
	//    if (amod++ % 1000 == 0) printf("Drag %f, vel %f\n", drag, v2);
    vect.x = drag * ptr->velocity.x;
	vect.y = drag * ptr->velocity.y;
	vect.z = drag * ptr->velocity.z;
}