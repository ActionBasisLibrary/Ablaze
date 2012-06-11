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
    double r0 = randf(0, 400);
    double r = 1600 / r0;
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
    *val = randf(2, 10);
}

void FountainCallback::acceleration(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
//    double buff[8];
//    transform->getValues("force", buff);
//    
//    gVector3f x = ptr->position;
//    x = x - gVector3f(512, 384, 0);
//    const gVector3f &v = ptr->velocity;
//    
//    vect.set(buff[0]*x.x + buff[1]*x.y + buff[2]*v.x + buff[3]*v.y,
//             buff[4]*x.x + buff[5]*x.y + buff[6]*v.x + buff[7]*v.y, 0.0);
//    vect *= 4.0;
    
    double vel[2];
    transform->getValues("positionCurve", vel, timer->getTime() - .1);
    
    double num;
    transform->getValues("numtouch", &num);
    
    double kx = num > 0 ? num : 0.0, kv = .3;
    vect.x = kx * (vel[0] - ptr->position.x) - kv * ptr->velocity.x;
    vect.y = kx * (vel[1] - ptr->position.y) - kv * ptr->velocity.y;
}

void FountainCallback::initOverride(ABParticles::Particle *ptr)
{
    ptr->birthTime = timer->getTime();
}