//
//  FountainController.m
//  Ablaze
//
//  Created by Caleb Jordan on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "FountainController.h"

#import "ABL/ABSymPull.h"
#import "ABL/ABSymVarPull.h"
#import "ABL/ABSymMean.h"
#import "ABL/ABSymCurve.h"
#import "ABL/ABSymTime.h"
#import "ABL/ABSymCombine.h"
#import "ABL/ABSymForceRegress.h"
#import "ABL/ABSymCard.h"
#import "ABL/ABSymIntegrate.h"
#import "ABL/ABSymDifferentiate.h"
#import "ABL/ABSymSmooth.h"
#import "ABL/ABCountIntegrate.h"

#include "GRand.h"


#pragma mark Particle System Setup
//ABLWrapper *_globalWrapper;

@interface FountainController()
-(void) prepTransform;
-(void) prepParticles;
@end


#pragma mark Global Functions

TouchState* _fountainTouchState;

bool _fountainUpdateX(double* buffer, unsigned int *count){
	return [_fountainTouchState getXs:buffer withCount:count];
}
bool _fountainUpdateY(double* buffer, unsigned int *count){
	return [_fountainTouchState getYs:buffer withCount:count];
}


@implementation FountainController
@synthesize particles, pshader, projectionMatrix;

- (void)setTouchState:(TouchState *)touchState {
	_fountainTouchState = touchState;
}
- (TouchState *)touchState{
	return _fountainTouchState;
}


- (void)setup
{
	// Create program
    
    const char *vertPath = [[[NSBundle mainBundle] pathForResource:@"ABParticles" ofType:@"vsh"] UTF8String];
    const char *fragPath = [[[NSBundle mainBundle] pathForResource:@"ABParticles" ofType:@"fsh"] UTF8String];
	
    // Creates the particle shader--see ABParticles.vsh, .fsh
    pshader = new ABParticleShader(vertPath, fragPath);
    
    [self prepTransform];
    [self prepParticles];
}

- (void)tearDown
{
    delete pshader;
    delete particles;
}

- (void)update:(NSTimeInterval)timeSinceLastUpdate
{	
    double num = 0;
    transform.getValues("inttouch", &num);
    
    double vel[2];
    transform.getValues("positionCurve", vel, timer.getTime() - .5, 1);
//    printf("Velocity: %f %f\n", vel[0], vel[1]);
    
//	int np = 50+sqrt(max(num, 0.0))*40;
//    particles->setNumContinuousParticles(min(np, 4000), pid0);
//	double linearVelocity = sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
	
//	double absVel = linearVelocity/1000;
//	if(absVel>1.0)absVel = 1.0;
//	if(absVel<0.0)absVel = 0.0;
	
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


- (void)prepTransform
{   
    transform = ABTransform(true);
    
    string names[] = {"x", "y", "meanX", "meanY", "position", "positionCurve", "inttouch", "currtouch", "force"};
	vector<string> position(names+2, names+4);
    
    vector<string> tickInputs(names+5, names+8);
//    vector<string> tickInputs2(names+7, names+8);
    
    ABSymbol *rawInputs[] = {
		new ABSymTime("time", &timer),
		new ABSymVarPull("x", MAX_TOUCH_COUNT, _fountainUpdateX),
		new ABSymVarPull("y", MAX_TOUCH_COUNT, _fountainUpdateY),
        
		new ABSymMean("meanX", "x"),
		new ABSymMean("meanY", "y"),
		new ABSymCombine("position", position),
        
        new ABSymCardNeg("numtouch", "x"),
        new ABSymIntegrate("inttouch", 1, "numtouch", "time"),
        new ABSymForceRegress<2>("force", "positionCurve", "time", 100),
        
        new ABCountIntegrate("currtouch", "x", "time")
	};
    
    ABSymCurve *curve = new ABSymCurve("positionCurve", 2, "position", "time", 6000, 900);
    
    ABSymTick *tick = new ABSymTick("tick", tickInputs, &self->timer, .001);
//    ABSymTick *ftick = new ABSymTick("ftick", tickInputs2, &self->timer, .01);
    
    transform.addSymbols(rawInputs, 10);
    transform.addSymbol(curve);
    transform.addSymbol(tick);
//    transform.addSymbol(ftick);
    
    transform.printSymbolDependencies(std::cout);
    
	transform.startTick("tick");
//    transform.startTick("ftick");
}

- (void)prepParticles
{
    // Initializes particle source with 2000 max capacity
    particles = new ABParticles(4000);
    
    callback.transform = &transform;
    callback.timer = &timer;
    callback.center.set(512, 384);
    
    odeCallback.transform = &transform;
    odeCallback.timer = &timer;
    odeCallback.center.set(512, 384);
    
    // Ignore this for now--none of it is used while debugging
    ABParticles::Profile profile;
    profile.lifeSpan = 6;
    profile.delay = 6;
    profile.continuous = true;
    profile.callback = &callback;
    
    pid0 = particles->createProfile(profile);
    
    ABParticles::Profile odeProfile;
    odeProfile.lifeSpan = 12;
    odeProfile.delay = 12;
    odeProfile.continuous = true;
    odeProfile.callback = &odeCallback;
    
    pidOde = particles->createProfile(odeProfile);
    
	particles->emitParticles(1000, pid0);
}

@end