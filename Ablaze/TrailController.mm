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
