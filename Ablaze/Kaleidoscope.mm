//
//  Kaleidoscope.m
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "Kaleidoscope.h"
#import "ABParticles.h"
#import "ABParticleShader.h"
#import "KaleidoscopeTransform.h"
#import "KaleidoscopeCallback.h"

@interface Kaleidoscope ()
{
	ABParticles *particleSystem;
	ABParticleShader *particleShader;
	KaleidoscopeTransform *transform;
	KaleidoscopeCallback *particleCallback;
}
@end

@implementation Kaleidoscope
@synthesize projectionMatrix, touchState;

-(void)setup
{
	transform = [KaleidoscopeTransform new];
	transform.touchState = self.touchState;
	
	// Create the particle shader
    const char *vertPath = [[[NSBundle mainBundle] pathForResource:@"ABParticles" ofType:@"vsh"] UTF8String];
    const char *fragPath = [[[NSBundle mainBundle] pathForResource:@"ABParticles" ofType:@"fsh"] UTF8String];
    particleShader = new ABParticleShader(vertPath, fragPath);
	
    // Initializes particle source
    particleSystem = new ABParticles(1000);
    
    // Define particles
    ABParticles::Profile profile;
    profile.lifeSpan = 10;
    profile.delay = 10;
    profile.continuous = true;
	
	particleCallback = new KaleidoscopeCallback();
	particleCallback->transform = transform;
	profile.callback = particleCallback;
    
    ABParticles::ProfileId pid = particleSystem->createProfile(profile);
    particleSystem->emitParticles(1000, pid);
}

-(void)tearDown
{
	free(particleSystem);
	free(particleShader);
	free(particleCallback);
	transform = nil;
}

-(void)update:(NSTimeInterval)timeSinceLastUpdate
{
	particleSystem->advanceParticlesBySeconds(timeSinceLastUpdate);
}

-(void)render
{
    particleShader->engage();
    particleShader->setTransform(((GLKMatrix4)GLKMatrix4Identity).m, self.projectionMatrix.m);
    particleSystem->engage();
    particleSystem->renderParticles();
    particleSystem->disengage();
    particleShader->disengage();
}



@end
