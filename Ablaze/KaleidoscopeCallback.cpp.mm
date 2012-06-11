//
//  KaleidoscopeCallback.cpp
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "KaleidoscopeCallback.h"
#include "GRand.h"

void KaleidoscopeCallback::startSize(float *size, float dt, const ABParticles::Particle *ptr)
{
	*size = 0.0;
}
void KaleidoscopeCallback::size(float *size, float dt, const ABParticles::Particle *ptr)
{
	float ageRatio = ptr->age/ptr->lifeSpan;
	*size = 100.0*(ageRatio-ageRatio*ageRatio);
}

void KaleidoscopeCallback::startColor(gVector4f &color, float dt, const ABParticles::Particle *ptr)
{
	color.set(0.0, 0.0, randf()*0.1+0.05, 1.0);
}

void KaleidoscopeCallback::startPosition(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
	vect.set(1024*randf(), 768*randf(), 0.0);
}
/*
void KaleidoscopeCallback::position(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
	double curvature = [transform getCurvature];
	if (curvature > 1000) {
		curvature = 1000;
	} else if (curvature <= 0) {
		curvature = 0.001;
	}
	vect.set(1/curvature, ptr->position.y, ptr->position.z);
}
*/

void KaleidoscopeCallback::startVelocity(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
	const float speed = (1.0-2.0*randf())*20.0;
	double angle = (2.0*M_PI*randf());
	vect.x = sin(angle)*speed;
	vect.y = cos(angle)*speed;
	vect.z = 0;
}

void KaleidoscopeCallback::velocity(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
	double curvature = [transform getCurvature];
	if (curvature > 1000) {
		curvature = 1000;
	} else if (curvature < 0) {
		curvature = 0;
	}
	double a = 1-0.005*curvature;
	double b = sqrt(1-a*a);
	vect.x = a*ptr->velocity.x-b*ptr->velocity.y;
	vect.y = a*ptr->velocity.y+b*ptr->velocity.x;
	vect.z = ptr->velocity.z;
	
	/*
	CGPoint velocity = [transform getVelocity:0.1];
	gVector2f currVelocity = gVector2f(velocity.x, velocity.y);
	
    double v2 = currVelocity.length() * .002;
    double drag = 1.0 * exp(-pow(v2 + .5, -4.0)) + 0.5;
	*/
}

void KaleidoscopeCallback::acceleration(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
	CGPoint velocity = [transform getVelocity:0.1];
	gVector2f currVelocity = gVector2f(velocity.x, velocity.y);
	
    double v2 = currVelocity.length() * .002;
    double drag = 1.5 * exp(-pow(v2 + .5, -4.0)) - 0.5;
	
    vect.x = drag * ptr->velocity.x;
	vect.y = drag * ptr->velocity.y;
	vect.z = drag * ptr->velocity.z;
}
