//
//  TrailController.m
//  Ablaze
//
//  Created by Matt Rubin on 6/9/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "TrailController.h"

#include "GRand.h"




#pragma mark Particle System Setup
ABLWrapper *_globalWrapper;

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



// Trail particle

void _trailInitOverrideFn(ABParticles::Particle *ptr) {
	// Use this for setting attributes that all depend on this randomized time
	float secondsAgo = 0.1+randf()*1.0;
	// Position
	CGPoint position = [_globalWrapper getPosition:secondsAgo];
	ptr->position.x = position.x-5+randf()*10;
	ptr->position.y = position.y-5+randf()*10;
	ptr->position.z = -5+randf()*10;
	
	// Color
	CGPoint velocity = [_globalWrapper getVelocity:secondsAgo];
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





@implementation TrailController
@synthesize particles, pshader, projectionMatrix;

- (void)setWrapper:(ABLWrapper *)wrapper {
	_globalWrapper = wrapper;
}
- (ABLWrapper *)wrapper{
	return _globalWrapper;
}


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
	profile.startVelFn = _startVelFunction;
	profile.startPosFn = _startPosFunction;
    profile.accFn = _dragAccFunction;
	profile.startColorFn = _startColorFn;
    
    ABParticles::ProfileId pid = particles->createProfile(profile);
    
    // This creates 100 particles--only important because it makes sure all positions are 0,0,0
    particles->emitParticles(000, pid);
	
	ABParticles::Profile trailProfile;
    trailProfile.lifeSpan = 2;
    trailProfile.delay = 2;
    trailProfile.continuous = true;
 	trailProfile.initOverrideFn = _trailInitOverrideFn;
   
    ABParticles::ProfileId trailPid = particles->createProfile(trailProfile);
	particles->emitParticles(1500, trailPid);
}

- (void)tearDown
{
	
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
	//NSLog(@"touchAdded: %@", key);
}

-(void)touchUpdated:(NSString*)key atPoint:(CGPoint)point
{
	//NSLog(@"touchUpdated: %@", key);
}

-(void)touchRemoved:(NSString*)key
{
	//NSLog(@"touchRemoved: %@", key);
}

@end
