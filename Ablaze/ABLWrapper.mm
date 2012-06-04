//
//  ABLWrapper.m
//  Ablaze
//
//  Created by Matt Rubin on 5/8/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "ABLWrapper.h"

#import "ABL/ABSymPull.h"
#import "ABL/ABSymVarPull.h"
#import "ABL/ABSymCurve.h"
#import "ABL/ABSymTime.h"
#import "ABL/ABSymCombine.h"
#import "ABL/ABSymDifferentiate.h"


@interface ABLWrapper ()
- (void)prepTransform;
@end


#pragma mark UI State Accessors

TouchState* globalTouchState;

bool updateX(double* buffer, unsigned int *count){
	return [globalTouchState getXs:buffer withCount:count];
}
bool updateY(double* buffer, unsigned int *count){
	return [globalTouchState getYs:buffer withCount:count];
}
bool updateTime(double* buffer){
	return true;
}

# pragma mark The Wrapper

@implementation ABLWrapper

-(id)init {
	if(self = [super init]){
		[self prepTransform];
	}
	return self;
}

#pragma mark Touch State Access

- (void)setTouchState:(TouchState *)touchState {
	globalTouchState = touchState;
}
- (TouchState *)touchState{
	return globalTouchState;
}

#pragma mark Transform Setup

- (void)prepTransform {
	transform = ABTransform(true);

	string names[] = {"x", "y", "xy", "curve", "velocity", "velCurve"};
	vector<string> vComp(names, names+2);

	ABSymbol *pos[] = {
		new ABSymVarPull("x", 11, updateX),
		new ABSymVarPull("y", 11, updateY),
		new ABSymCombine("xy", vComp),
		new ABSymTime("time", &timer, 0)
	};

	vector<string> vVel(names, names+2);
	ABSymbol *velocity = new ABSymDifferentiate("velocity", vVel, "time");
	ABSymbol *velocityCurve = new ABSymCurve("velCurve", 2, "velocity", "time", 100, 5);

	vector<string> vTick(names+2, names+6);
	ABSymbol *tick = new ABSymTick("tick", vTick, &timer, 0.1);

	ABSymbol *curve = new ABSymCurve("curve", 2, "xy", "time", 100, 5);

	transform.addSymbols(pos, 4);
	transform.addSymbol(tick);
	transform.addSymbol(curve);

	transform.addSymbol(velocity);
	transform.addSymbol(velocityCurve);

	transform.startTick("tick");
}

#pragma mark Transform Out

- (CGPoint)getMean:(float)secondsAgo {
	double buffer[2];
	
	if(secondsAgo == 0.0){
		transform.getValues("xy", buffer);
	} else {
		float time = timer.getTime()-secondsAgo;
		transform.getValues("curve", buffer, time);
	}
	
	return CGPointMake(buffer[0], buffer[1]);
}

- (CGPoint)getVelocity:(float)secondsAgo {
	double buffer[2];
	
	if(secondsAgo == 0.0){
		transform.getValues("velocity", buffer);
	} else {
		float time = timer.getTime()-secondsAgo;
		transform.getValues("velCurve", buffer, time);
	}
	
	return CGPointMake(buffer[0], buffer[1]);
}

@end
