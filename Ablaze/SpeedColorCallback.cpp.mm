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
	CGPoint point = [wrapper getPosition:0.1];
	vect.set(point.x, point.y, 0.0);
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
	CGPoint velocity = [wrapper getVelocity:0.1];
	double linearVelocity = sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
	
	double absVel = linearVelocity/1000;
	if(absVel>1.0)absVel = 1.0;
	if(absVel<0.0)absVel = 0.0;
	
	//NSLog(@"%f, %f = %f", velocity.x, velocity.y, absVel);
	vect.r = absVel;
	vect.g = 0.0;
	vect.b = 1.0-absVel;
	vect.a = 1.0;
}

void SpeedColorCallback::acceleration(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
	CGPoint velocity = [wrapper getVelocity:0.1];
	gVector2f currVelocity = gVector2f(velocity.x, velocity.y);

    double v2 = currVelocity.length() * .001;
    double drag = 1.5 * exp(-pow(v2 + .5, -4.0)) - 1.0;
	//    if (amod++ % 1000 == 0) printf("Drag %f, vel %f\n", drag, v2);
    vect.x = drag * ptr->velocity.x;
	vect.y = drag * ptr->velocity.y;
	vect.z = drag * ptr->velocity.z;
}