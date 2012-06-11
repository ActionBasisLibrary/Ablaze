//
//  KaleidoscopeCallback.cpp
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "KaleidoscopeCallback.h"
#include "GRand.h"


void KaleidoscopeCallback::size(float *size, float dt, const ABParticles::Particle *ptr)
{
	float ageRatio = ptr->age/ptr->lifeSpan;
	*size = 100.0*(ageRatio-ageRatio*ageRatio);
}

void KaleidoscopeCallback::startColor(gVector4f &color, float dt, const ABParticles::Particle *ptr)
{
	color.set(0.0, randf()*0.1+0.05, randf()*0.1+0.05, 1.0);
}

void KaleidoscopeCallback::startPosition(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
	vect.set(1024*randf(), 768*randf(), 0.0);
}

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
	
	const double weight = 0.005;
	if (curvature > 1/weight) {
		curvature = 1/weight;
	} else if (curvature < 0) {
		curvature = 0;
	}
	
	double a = 1-0.005*curvature;
	if (a < 0.0) a = 0.0;
	if (a > 1.0) a = 1.0;
	double b = sqrt(1-a*a);
	
	vect.x = a*ptr->velocity.x-b*ptr->velocity.y;
	vect.y = a*ptr->velocity.y+b*ptr->velocity.x;
	vect.z = ptr->velocity.z;
}
