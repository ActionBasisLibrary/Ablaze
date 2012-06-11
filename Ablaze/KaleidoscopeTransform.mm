//
//  KaleidoscopeTransform.m
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "KaleidoscopeTransform.h"
#import "ABL/ABTransform.h"
#import "ABL/ABSymPull.h"
#import "ABL/ABSymVarPull.h"
#import "ABL/ABSymMean.h"
#import "ABL/ABSymCurve.h"
#import "ABL/ABSymTime.h"
#import "ABL/ABSymCombine.h"
#import "ABL/ABSymDifferentiate.h"
#import "ABL/ABSymCurvature.h"

#pragma mark UI State Accessors

TouchState* _kaleidoscopeGlobalTouchState;

bool kaleidoscopeUpdateX(double* buffer, unsigned int *count){
	return [_kaleidoscopeGlobalTouchState getXs:buffer withCount:count];
}
bool kaleidoscopeUpdateY(double* buffer, unsigned int *count){
	return [_kaleidoscopeGlobalTouchState getYs:buffer withCount:count];
}


@interface KaleidoscopeTransform ()
{
	ABTransform transform;
	GTimerThread timer;
}
- (void)prepTransform;
@end

@implementation KaleidoscopeTransform

-(id)init {
	if(self = [super init]){
		[self prepTransform];
	}
	return self;
}

#pragma mark Touch State Access

- (void)setTouchState:(TouchState *)touchState {
	_kaleidoscopeGlobalTouchState = touchState;
}
- (TouchState *)touchState{
	return _kaleidoscopeGlobalTouchState;
}

#pragma mark Transform Setup

- (void)prepTransform {
	
	string names[] = {"x", "y", "meanX", "meanY", "position", "positionCurve", "velocity", "velocityCurve", "curvature"};
	vector<string> position(names+2, names+4);
	
	ABSymbol *rawInputs[] = {
		new ABSymTime("time", &timer),
		new ABSymVarPull("x", MAX_TOUCH_COUNT, kaleidoscopeUpdateX),
		new ABSymVarPull("y", MAX_TOUCH_COUNT, kaleidoscopeUpdateY),
		new ABSymMean("meanX", "x"),
		new ABSymMean("meanY", "y"),
		new ABSymCombine("position", position),
	};
	
	ABSymbol *velocity = new ABSymDifferentiate("velocity", position, "time");
	ABSymbol *velocityCurve = new ABSymCurve("velocityCurve", 2, "velocity", "time", 100, 5);
	
	vector<string> vTick(names+2, names+9);
	ABSymbol *tick = new ABSymTick("tick", vTick, &timer, 0.1);
	
	ABSymbol *positionCurve = new ABSymCurve("positionCurve", 2, "position", "time", 100, 5);
	
	ABSymbol *curvature = new ABSymCurvature("curvature", "position", 2, "time");

	// Create a new transform and add all of the symbols
	transform = ABTransform(true);
	
	transform.addSymbols(rawInputs, 6);
	transform.addSymbol(positionCurve);
	
	transform.addSymbol(velocity);
	transform.addSymbol(velocityCurve);
	
	transform.addSymbol(curvature);
	
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

- (double)getCurvature {
	double curvature;
	
	transform.getValues("curvature", &curvature);
	
	return curvature;
}

-(void)printCurve: (NSNotification *) notification
{
    transform.printCurveData("velocityCurve");
}

@end
