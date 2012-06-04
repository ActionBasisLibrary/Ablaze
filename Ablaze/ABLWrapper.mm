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

	string names[] = {"x", "y", "position", "positionCurve", "velocity", "velocityCurve"};
	vector<string> position(names, names+2);

	ABSymbol *rawInputs[] = {
		new ABSymTime("time", &timer, 0),
		new ABSymVarPull("x", MAX_TOUCH_COUNT, updateX),
		new ABSymVarPull("y", MAX_TOUCH_COUNT, updateY),
		new ABSymCombine("position", position),
	};

	ABSymbol *velocity = new ABSymDifferentiate("velocity", position, "time");
	ABSymbol *velocityCurve = new ABSymCurve("velocityCurve", 2, "velocity", "time", 100, 5);

	vector<string> vTick(names+2, names+6);
	ABSymbol *tick = new ABSymTick("tick", vTick, &timer, 0.1);

	ABSymbol *positionCurve = new ABSymCurve("positionCurve", 2, "position", "time", 100, 5);


	// Create a new transform and add all of the symbols
	transform = ABTransform(true);

	transform.addSymbols(rawInputs, 4);
	transform.addSymbol(positionCurve);

	transform.addSymbol(velocity);
	transform.addSymbol(velocityCurve);

	transform.addSymbol(tick);
	transform.startTick("tick");
}

#pragma mark Transform Out

- (CGPoint)getMean:(float)secondsAgo {
	double buffer[2];
	
	if(secondsAgo == 0.0){
		transform.getValues("position", buffer);
	} else {
		float time = timer.getTime()-secondsAgo;
		transform.getValues("positionCurve", buffer, time);
	}
	
	return CGPointMake(buffer[0], buffer[1]);
}

- (CGPoint)getVelocity:(float)secondsAgo {
	double buffer[2];
	
	if(secondsAgo == 0.0){
		transform.getValues("velocity", buffer);
	} else {
		float time = timer.getTime()-secondsAgo;
		transform.getValues("velocityCurve", buffer, time);
	}
	
	return CGPointMake(buffer[0], buffer[1]);
}

@end
