//
//  ODECallback.m
//  Ablaze
//
//  Created by Caleb Jordan on 6/11/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "ODECallback.h"

#include "GRand.h"
#include "GColorUtils.h"

void ODECallback::startPosition(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
    double r0 = randf(0,1);
    double r = 800 - 800 * r0*r0;
//    double r = 50;
    double th = randf(0, 2*M_PI);
    
    vect.set(center.x + r*cos(th), center.y + r*sin(th), 0);
}

void ODECallback::startVelocity(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
    vect.set(0,0,0);
}

void ODECallback::startColor(gVector4f &vect, float dt, const ABParticles::Particle *ptr)
{
    double qvals[5];
    transform->getValues("quad", qvals);
    
    double r = randd(0,1);
    
    if (qvals[4] < 1e-6) {
        vect.set(1, 1, 1);
    }
    
    else if (r < qvals[0]/qvals[4]) {
        vect.set(1, 0, 0);
    }
    
    else if (r < qvals[1]/qvals[4]) {
        vect.set(0, 1, 0);
    }
    
    else if (r < qvals[2]/qvals[4]) {
        vect.set(1, 1, 0);
    }
    
    else {
        vect.set(0, 0, 1);
    }
}

//void ODECallback::color(gVector4f &vect, float dt, const ABParticles::Particle *ptr)
//{
//    
//}

void ODECallback::startSize (float *val, float dt, const ABParticles::Particle *ptr)
{
    *val = randf(4, 12);
}

void ODECallback::acceleration(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
//    vect.set(0,0,0);
    
    double coef[8];
    transform->getValues("force", coef);
    
    gVector3f x = ptr->position;
    gVector3f v = ptr->velocity;
    
    x.x -= center.x;
    x.y -= center.y;
    
    vect.set(x.x*coef[0] + x.y*coef[1] + v.x*coef[2] + v.y*coef[3],
             x.x*coef[4] + x.y*coef[5] + v.x*coef[6] + v.y*coef[7],
             0.0);
}

void ODECallback::initOverride(ABParticles::Particle *ptr)
{
    ptr->birthTime = timer->getTime();
}