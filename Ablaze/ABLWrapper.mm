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
#import "ABL/ABSymMean.h"
#import "ABL/ABSymCurve.h"
#import "ABL/ABSymTime.h"
#import "ABL/ABSymCombine.h"
#import "ABL/ABSymDifferentiate.h"
#import <iostream>


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
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(printCurve:)
		 name:@"applicationWillResignActive"
		 object:nil ];

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

	string names[] = {"x", "y", "meanX", "meanY", "position", "positionCurve", "velocity", "velocityCurve"};
	vector<string> position(names+2, names+4);

	ABSymbol *rawInputs[] = {
		new ABSymTime("time", &timer),
		new ABSymVarPull("x", MAX_TOUCH_COUNT, updateX),
		new ABSymVarPull("y", MAX_TOUCH_COUNT, updateY),
		new ABSymMean("meanX", "x"),
		new ABSymMean("meanY", "y"),
		new ABSymCombine("position", position),
	};

	ABSymbol *velocity = new ABSymDifferentiate("velocity", 2, "position", "time");
	ABSymbol *velocityCurve = new ABSymCurve("velocityCurve", 2, "velocity", "time", 100, 5);

	vector<string> vTick(names+2, names+8);
	ABSymbol *tick = new ABSymTick("tick", vTick, &timer, 0.1);

	ABSymbol *positionCurve = new ABSymCurve("positionCurve", 2, "position", "time", 100, 5);


	// Create a new transform and add all of the symbols
	transform = ABTransform(true);

	transform.addSymbols(rawInputs, 6);
	transform.addSymbol(positionCurve);

	transform.addSymbol(velocity);
	transform.addSymbol(velocityCurve);

	transform.addSymbol(tick);
    
//    transform.printSymbolDependencies(std::cout);
    
	transform.startTick("tick");
    
//    transform.printSymbolDependencies(std::cout);
}

#pragma mark Transform Out

- (CGPoint)getPosition:(float)secondsAgo {
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



-(void)printCurve: (NSNotification *) notification
{
    transform.printCurveData("velocityCurve");
}
@end
