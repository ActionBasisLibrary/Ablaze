//
//  TrailController.m
//  Ablaze
//
//  Created by Matt Rubin on 6/9/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "TrailController.h"

#import "ABParticles.h"
#import "ABParticleShader.h"

#include "RainbowTrailCallback.h"
#include "SpeedColorCallback.h"


@interface TrailController ()
{
	ABParticles *particles;
	ABParticleShader *pshader;
	RainbowTrailCallback *trailCallback;
	SpeedColorCallback *instantCallback;
}
@end


@implementation TrailController
@synthesize projectionMatrix, wrapper;

- (void)setup
{
	// Create the particle shader
    const char *vertPath = [[[NSBundle mainBundle] pathForResource:@"ABParticles" ofType:@"vsh"] UTF8String];
    const char *fragPath = [[[NSBundle mainBundle] pathForResource:@"ABParticles" ofType:@"fsh"] UTF8String];
    pshader = new ABParticleShader(vertPath, fragPath);
	
    // Initializes particle source with 2000 max capacity
    particles = new ABParticles(2000);
    
    // Define instantaneous particles
    ABParticles::Profile profile;
    profile.lifeSpan = 2;
    profile.delay = 2;
    profile.continuous = true;
	
	instantCallback = new SpeedColorCallback();
	instantCallback->wrapper = self.wrapper;
	profile.callback = instantCallback;
    
    ABParticles::ProfileId pid = particles->createProfile(profile);
    particles->emitParticles(500, pid);
	
    // Define trail particles
	ABParticles::Profile trailProfile;
    trailProfile.lifeSpan = 2;
    trailProfile.delay = 2;
    trailProfile.continuous = true;
	
	trailCallback = new RainbowTrailCallback();
	trailCallback->wrapper = self.wrapper;
	trailProfile.callback = trailCallback;
   
    ABParticles::ProfileId trailPid = particles->createProfile(trailProfile);
	particles->emitParticles(1500, trailPid);
}

- (void)tearDown
{
	free(particles);
	free(pshader);
	free(trailCallback);
	free(instantCallback);
}

- (void)update:(NSTimeInterval)timeSinceLastUpdate
{	
	particles->advanceParticlesBySeconds(timeSinceLastUpdate);
}

- (void)render
{
    pshader->engage(); // Sets current program to use
    pshader->setTransform(((GLKMatrix4)GLKMatrix4Identity).m,
								 self.projectionMatrix.m); // Sets the modelview and projection uniforms
	
    particles->engage(); // Sets the vertex attribute pointers
    particles->renderParticles(); // Draw the particles
    particles->disengage(); // Turns off used vertex attribute arrays
	
    pshader->disengage();
}

@end
