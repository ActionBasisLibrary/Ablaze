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

TouchState* ts;

@implementation ABLWrapper
@synthesize touchState;



bool updateX(double* buffer, unsigned int *count){
	bool a = [ts getXs:buffer withCount:count];
	
//	printf("X: ");
//	for (int i=0; i<*count; i++) {
//		printf("%.1f", buffer[i]);
//		if(i<*count-1) printf(", ");
//	}
//	printf("\n");
	
	return a;
}
bool updateY(double* buffer, unsigned int *count){
	bool a = [ts getYs:buffer withCount:count];
	
//	printf("Y: ");
//	for (int i=0; i<*count; i++) {
//		printf("%.1f", buffer[i]);
//		if(i<*count-1) printf(", ");
//	}
//	printf("\n");
	
	return a;
}

bool updateTime(double* buffer){
	return true;
}

-(void)prepTouchState
{
	ts = touchState;
}

-(id)init {
	if(self = [super init]){
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
	return self;
}

-(CGPoint)getMean {
	double buffer[2];
	//double time = timer.getTime()-1.0;
	//transform.getValues("curve", buffer, time);
	transform.getValues("xy", buffer);
	
	return CGPointMake(buffer[0], buffer[1]);
}

-(CGPoint)getVelocity {
	double buffer[2];
	//double time = timer.getTime()-1.0;
	//transform.getValues("velCurve", buffer, time);
	transform.getValues("velocity", buffer);
	
	return CGPointMake(buffer[0], buffer[1]);
}




@end


