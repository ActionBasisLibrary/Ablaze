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
#import "ABL/ABSymCurveCurvature.h"

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
	
	string names[] = {"x", "y","position", "positionCurve", "curvature"};
	
	vector<string> position(names, names+2);
	vector<string> vTick(names+2, names+5);
	
	ABSymbol *rawInputs[] = {
		new ABSymTime("time", &timer),
		new ABSymVarPull("x", MAX_TOUCH_COUNT, kaleidoscopeUpdateX),
		new ABSymVarPull("y", MAX_TOUCH_COUNT, kaleidoscopeUpdateY),
		new ABSymCombine("position", position),
		new ABSymCurve("positionCurve", 2, "position", "time", 100, 5),
		new ABSymCurveCurvature("curvature", "positionCurve", 2, "time"),
		new ABSymTick("tick", vTick, &timer, 0.1),
	};

	// Create a new transform and add all of the symbols
	transform = ABTransform(true);
	transform.addSymbols(rawInputs, 7);
	transform.startTick("tick");
}

- (double)getCurvature {
	double curvature;
	transform.getValues("curvature", &curvature);
	return curvature;
}

@end
