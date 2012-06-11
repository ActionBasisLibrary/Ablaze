//
//  FountainCallback.m
//  Ablaze
//
//  Created by Caleb Jordan on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "FountainCallback.h"

#include "GRand.h"
#include "GColorUtils.h"

void FountainCallback::startPosition(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
    double r0 = randf(1, 400);
    double r = 600 / r0;
    double th = randf(0, 2*M_PI);
    
    vect.set(center.x + r*cos(th), center.y + r*sin(th), 0);
}

void FountainCallback::startVelocity(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
//	const float speed = (1.0-2.0*randf())*100.0;
//	double angle = (2.0*M_PI*randf());
//    vect.set(sin(angle)*speed, cos(angle)*speed, 0);
    vect.set(0,0,0);
}

void FountainCallback::color(gVector4f &vect, float dt, const ABParticles::Particle *ptr)
{
    double v = ((gVector3f)ptr->velocity).length();
    double h = atan(v * .02 - 10) / M_PI + .5;
    gVector3f rgb = gColorUtils::getRGB(h, 1, .5);
    vect.set(rgb);
}

void FountainCallback::startSize (float *val, float dt, const ABParticles::Particle *ptr)
{
    double num;
    transform->getValues("currtouch", &num);
    *val = randf(4.0, max(4.0,num));
}

void FountainCallback::acceleration(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
    double pos[2];
    transform->getValues("positionCurve", pos, timer->getTime() - .1);
    
    double num;
    transform->getValues("numtouch", &num);
    double inum;
    transform->getValues("inttouch", &inum);
    
    double vel[2];
    transform->getValues("positionCurve", vel, timer->getTime() - .1, 1);
//    double vsq = sqrtf(vel[0]*vel[0]+vel[1]*vel[1]);
    
    double kx = num > 0 ? num : 0.0, kv = -.2;//5 + 1.5 / max(1.0, inum*.1);
    vect.x = kx * (pos[0] - ptr->position.x) - kv * ptr->velocity.x + vel[0];
    vect.y = kx * (pos[1] - ptr->position.y) - kv * ptr->velocity.y + vel[1];
}

void FountainCallback::initOverride(ABParticles::Particle *ptr)
{
    ptr->birthTime = timer->getTime();
}