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

void ODECallback::color(gVector4f &vect, float dt, const ABParticles::Particle *ptr)
{
    
    vect.set(.8, .7, .2);
//    double v = ((gVector3f)ptr->velocity).length();
//    double h = atan(v * .02 - 10) / M_PI + .5;
//    gVector3f rgb = gColorUtils::getRGB(h, 1, .5);
//    vect.set(rgb);
}

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