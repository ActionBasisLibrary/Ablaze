//
//  TouchTrail.m
//  Ablaze
//
//  Created by Matt Rubin on 6/9/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "TouchTrail.h"

@implementation TouchTrail
@synthesize position;


- (void)setupProfile
{
    profile.lifeSpan = 2;
    profile.delay = 2;
    profile.continuous = true;

	callback = new TouchTrailCallback();
	profile.callback = callback;
}

- (void)tearDownProfile
{
	profile.callback = NULL;
	if (callback) free(callback);
}

- (void)attachToParticleSystem:(ABParticles*)particleSystem
{
	[self setupProfile];
	
	profileID = particleSystem->createProfile(profile);
	particleSystem->emitParticles(100, profileID);
}

- (void)detachFromParticleSystem:(ABParticles*)particleSystem
{
	[self tearDownProfile];
	
	particleSystem->setNumContinuousParticles(0, profileID);
}

@end
