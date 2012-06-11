//
//  TrailController.m
//  Ablaze
//
//  Created by Matt Rubin on 6/9/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "TrailController.h"
#import "TouchTrail.h"

#include "GRand.h"




#pragma mark Particle System Setup

gVector3f _particleStartPosition;
gVector2f _currVelocity;
gVector4f _particleStartColor;

void _startPosFunction(gVector3f &vect, float dt, const ABParticles::Particle *ptr) {
	vect = _particleStartPosition;
}
void _startVelFunction(gVector3f &vect, float dt, const ABParticles::Particle *ptr) {
	const float speed = (1.0-2.0*randf())*100.0;
	double angle = (2.0*M_PI*randf());
	vect.x = sin(angle)*speed;
	vect.y = cos(angle)*speed;
	vect.z = 0;
}
void _startColorFn(gVector4f &vect, float dt, const ABParticles::Particle *ptr) {
	vect = _particleStartColor;
}

int _amod = 0;
void _dragAccFunction(gVector3f &vect, float dt, const ABParticles::Particle *ptr) {
    double v2 = _currVelocity.length() * .001;
    double drag = 1.5 * exp(-pow(v2 + .5, -4.0)) - 1.0;
	//    if (amod++ % 1000 == 0) printf("Drag %f, vel %f\n", drag, v2);
    vect.x = drag * ptr->velocity.x;
	vect.y = drag * ptr->velocity.y;
	vect.z = drag * ptr->velocity.z;
}





@interface TrailController ()
{
	NSMutableDictionary *trails;
}
@end



@implementation TrailController
@synthesize particles, pshader, projectionMatrix;
@synthesize wrapper;


- (void)setup
{
	trails = [[NSMutableDictionary alloc] initWithCapacity:11];
	
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
	//profile.startVelFn = _startVelFunction;
	//profile.startPosFn = _startPosFunction;
    //profile.accFn = _dragAccFunction;
	//profile.startColorFn = _startColorFn;
    
    ABParticles::ProfileId pid = particles->createProfile(profile);
    
    // This creates 100 particles--only important because it makes sure all positions are 0,0,0
    particles->emitParticles(000, pid);
	
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
	trails = nil;
	free(particles);
	free(pshader);
	free(trailCallback);
}

- (void)update:(NSTimeInterval)timeSinceLastUpdate
{	
	CGPoint point = [self.wrapper getPosition:0.1];
    _particleStartPosition.set(point.x, point.y, 0.0);
	
	CGPoint velocity = [self.wrapper getVelocity:0.1];
	//    printf("Velocity @ -%f = %f %f\n", 0.1, velocity.x, velocity.y);
    _currVelocity.set(velocity.x, velocity.y);
	double linearVelocity = sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
	
	double absVel = linearVelocity/1000;
	if(absVel>1.0)absVel = 1.0;
	if(absVel<0.0)absVel = 0.0;
	
	//NSLog(@"%f, %f = %f", velocity.x, velocity.y, absVel);
	_particleStartColor.r = absVel;
	_particleStartColor.g = 0.0;
	_particleStartColor.b = 1.0-absVel;
	_particleStartColor.a = 1.0;

	
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
-(void)touchAdded:(NSString*)key atPoint:(CGPoint)point
{
/*	TouchTrail *newTrail = [TouchTrail new];
	newTrail.position = point;
	[newTrail attachToParticleSystem:particles];
	
	[trails setObject:newTrail forKey:key];
*/
}

-(void)touchUpdated:(NSString*)key atPoint:(CGPoint)point
{
/*	TouchTrail *updatedTrail = [trails objectForKey:key];
	updatedTrail.position = point;
*/
}

-(void)touchRemoved:(NSString*)key
{
/*	TouchTrail *removedTrail = [trails objectForKey:key];
	[removedTrail detachFromParticleSystem:particles];
	[trails removeObjectForKey:key];
*/
}

@end
