//
//  ChaosController.m
//  Ablaze
//
//  Created by Caleb Jordan on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "ChaosController.h"

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
#import "ABL/ABQuadIntegrator.h"

#include "GRand.h"


#pragma mark Particle System Setup
//ABLWrapper *_globalWrapper;

@interface ChaosController()
-(void) prepTransform;
-(void) prepParticles;
@end


#pragma mark Global Functions

TouchState* _chaosTouchState;

bool _chaosUpdateX(double* buffer, unsigned int *count){
	return [_chaosTouchState getXs:buffer withCount:count];
}
bool _chaosUpdateY(double* buffer, unsigned int *count){
	return [_chaosTouchState getYs:buffer withCount:count];
}


@implementation ChaosController
@synthesize particles, pshader, projectionMatrix;

- (void)setTouchState:(TouchState *)touchState {
	_chaosTouchState = touchState;
}
- (TouchState *)touchState{
	return _chaosTouchState;
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
    double vel[2];
    transform.getValues("positionCurve", vel, timer.getTime() - .5, 1);
    //    printf("Velocity: %f %f\n", vel[0], vel[1]);
    
//    particles->setNumContinuousParticles(sqrt(max(num, 0.0))*40, pid0);
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
    double ctr[] = {512, 368};
    
    string names[] = {"x", "y", "meanX", "meanY", "position", "positionCurve", "quad", "force"};
	vector<string> position(names+2, names+4);
    
    vector<string> tickInputs(names+5, names+7);
    vector<string> tickInputs2(names+7, names+8);
    
    ABSymbol *rawInputs[] = {
		new ABSymTime("time", &timer),
		new ABSymVarPull("x", MAX_TOUCH_COUNT, _chaosUpdateX),
		new ABSymVarPull("y", MAX_TOUCH_COUNT, _chaosUpdateY),
        
		new ABSymMean("meanX", "x"),
		new ABSymMean("meanY", "y"),
		new ABSymCombine("position", position),
        
        new ABQuadIntegrator("quad", "position", "time", ctr),
        
        new ABSymForceRegress<2>("force", "positionCurve", "time", 100)
	};
    
    ABSymCurve *curve = new ABSymCurve("positionCurve", 2, "position", "time", 6000, 900);
    
    ABSymTick *tick = new ABSymTick("tick", tickInputs, &self->timer, .001);
    ABSymTick *ftick = new ABSymTick("ftick", tickInputs2, &self->timer, .01);
    
    transform.addSymbols(rawInputs, 8);
    transform.addSymbol(curve);
    transform.addSymbol(tick);
    transform.addSymbol(ftick);
    
    transform.printSymbolDependencies(std::cout);
    
	transform.startTick("tick");
    transform.startTick("ftick");
}

- (void)prepParticles
{
    // Initializes particle source with 2000 max capacity
    particles = new ABParticles(4000);
    
    callback.transform = &transform;
    callback.timer = &timer;
    callback.center.set(512, 384);
    
    // Ignore this for now--none of it is used while debugging
    
    ABParticles::Profile odeProfile;
    odeProfile.lifeSpan = 12;
    odeProfile.delay = 12;
    odeProfile.continuous = true;
    odeProfile.callback = &callback;
    
    pid0 = particles->createProfile(odeProfile);
    
    particles->emitParticles(1500, pid0);
}

@end