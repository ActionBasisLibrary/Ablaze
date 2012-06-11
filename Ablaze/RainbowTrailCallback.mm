//
//  RainbowTrailCallback.cpp
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "RainbowTrailCallback.h"
#include "GRand.h"


void RainbowTrailCallback::initOverride(ABParticles::Particle *ptr)
{
	// Use this for setting attributes that all depend on this randomized time
	float secondsAgo = 0.1+randf()*1.0;
	// Position
	CGPoint position = [wrapper getPosition:secondsAgo];
	ptr->position.x = position.x-5+randf()*10;
	ptr->position.y = position.y-5+randf()*10;
	ptr->position.z = -5+randf()*10;
	
	// Color
	CGPoint velocity = [wrapper getVelocity:secondsAgo];
	//double linearVelocity = sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
	//double absVel = linearVelocity/1000-0.1;
	//if(absVel>1.0)absVel = 1.0;
	//if(absVel<0.0)absVel = 0.0;
	double H = atan2(velocity.y, velocity.x);
	if (H<0)
		H += 2.0*M_PI;
	H /= 2.0*M_PI;
	[[UIColor colorWithHue:H saturation:1.0 brightness:1.0 alpha:1.0] getRed:&(ptr->color.r) green:&(ptr->color.g) blue:&(ptr->color.b) alpha:&(ptr->color.a)];
	/*	printf(" %f\n", H);
	 if (H<0.0) {
	 printf("\n\n\n!!! H<0.0 !!!\n\n\n");
	 } else if (H>=2.0*M_PI) {
	 printf("\n\n\n!!! H>=2.0*M_PI !!!\n\n\n");
	 }
	 H /= (M_PI/3.0);
	 double X = 1-fabs((H/2)-floor(H/2) - 1);
	 double C = 1.0;
	 
	 if (X < 1.0) {
	 ptr->color.set(C, X, 0.0);
	 } else if (X < 2.0) {
	 ptr->color.set(X, C, 0.0);
	 } else if (X < 3.0) {
	 ptr->color.set(0.0, C, X);
	 } else if (X < 4.0) {
	 ptr->color.set(0.0, X, C);
	 } else if (X < 5.0) {
	 ptr->color.set(X, 0.0, C);
	 } else if (X < 6.0) {
	 ptr->color.set(C, 0.0, X);
	 }
	 */	
	//	ptr->color.r = absVel;
	//	ptr->color.g = 0.0;
	//	ptr->color.b = 1.0-absVel;
	//	ptr->color.a = 1.0;
	
	// Size
	ptr->size = (position.y/1000)*30.0f+5.0f;
	
	// Velocity
	double vel = (position.x/1000)*randf()*30.0+5.0;
	double angle = (2.0*M_PI*randf());
	ptr->velocity.x = vel*cos(angle);
	ptr->velocity.y = vel*sin(angle);
	ptr->velocity.z = 0;
}
