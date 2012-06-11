//
//  ABParticles_ProfileCallback.cpp
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABParticles.h"

void ABParticles::ProfileCallback::initOverride (ABParticles::Particle *ptr)
{}

void ABParticles::ProfileCallback::size (float *size, float dt, const ABParticles::Particle *ptr)
{}

void ABParticles::ProfileCallback::position (gVector3f &position, float dt, const ABParticles::Particle *ptr)
{
    position += (gVector3f)ptr->velocity * dt;
}

void ABParticles::ProfileCallback::velocity (gVector3f &velocity, float dt, const ABParticles::Particle *ptr)
{
    velocity += (gVector3f)ptr->acceleration * dt;
}

void ABParticles::ProfileCallback::acceleration (gVector3f &acceleration, float dt, const ABParticles::Particle *ptr)
{}

void ABParticles::ProfileCallback::color (gVector4f &color, float dt, const ABParticles::Particle *ptr)
{}



void ABParticles::ProfileCallback::startSize (float *size, float dt, const ABParticles::Particle *ptr)
{
	*size = 10.0;
}

void ABParticles::ProfileCallback::startPosition (gVector3f &position, float dt, const ABParticles::Particle *ptr)
{
	position = gVector3f(0);
}

void ABParticles::ProfileCallback::startVelocity (gVector3f &velocity, float dt, const ABParticles::Particle *ptr)
{
	velocity = gVector3f(0);
}

void ABParticles::ProfileCallback::startAcceleration (gVector3f &acceleration, float dt, const ABParticles::Particle *ptr)
{
	acceleration = gVector3f(0);
}

void ABParticles::ProfileCallback::startColor (gVector4f &color, float dt, const ABParticles::Particle *ptr)
{
	color = gVector4f(1);
}

