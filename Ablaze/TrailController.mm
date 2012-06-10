//
//  TrailController.m
//  Ablaze
//
//  Created by Matt Rubin on 6/9/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "TrailController.h"

@implementation TrailController
@synthesize particles, pshader, projectionMatrix;


- (void)setup
{
	// Create program
    
    const char *vertPath = [[[NSBundle mainBundle] pathForResource:@"ABParticles" ofType:@"vsh"] UTF8String];
    const char *fragPath = [[[NSBundle mainBundle] pathForResource:@"ABParticles" ofType:@"fsh"] UTF8String];
	
    // Creates the particle shader--see ABParticles.vsh, .fsh
    pshader = new ABParticleShader(vertPath, fragPath);
	
    // Initializes particle source with 2000 max capacity
    particles = new ABParticles(2000);
    
    // Ignore this for now--none of it is used while debugging
    ABParticles::Profile profile;
    profile.lifeSpan = 2;
    profile.delay = 2;
    profile.continuous = true;
    
    ABParticles::ProfileId pid = particles->createProfile(profile);
    
    // This creates 100 particles--only important because it makes sure all positions are 0,0,0
    particles->emitParticles(000, pid);
	
	ABParticles::Profile trailProfile;
    trailProfile.lifeSpan = 2;
    trailProfile.delay = 2;
    trailProfile.continuous = true;
    
    ABParticles::ProfileId trailPid = particles->createProfile(trailProfile);
	particles->emitParticles(1500, trailPid);
}

- (void)tearDown
{
	
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

# pragma mark - TouchStateDelegate methods
-(void)touchAdded:(NSString*)key
{
	NSLog(@"touchAdded: %@", key);
}

-(void)touchUpdated:(NSString*)key
{
	NSLog(@"touchUpdated: %@", key);
}

-(void)touchRemoved:(NSString*)key
{
	NSLog(@"touchRemoved: %@", key);
}

@end
