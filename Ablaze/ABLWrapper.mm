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

TouchState* ts;

@implementation ABLWrapper
@synthesize touchState;



bool updateX(double* buffer, unsigned int *count){
	bool a = [ts getXs:buffer withCount:count];
	
	printf("X: ");
	for (int i=0; i<*count; i++) {
		printf("%.1f", buffer[i]);
		if(i<*count-1) printf(", ");
	}
	printf("\n");
	
	return a;
}
bool updateY(double* buffer, unsigned int *count){
	bool a = [ts getYs:buffer withCount:count];
	
	printf("Y: ");
	for (int i=0; i<*count; i++) {
		printf("%.1f", buffer[i]);
		if(i<*count-1) printf(", ");
	}
	printf("\n");
	
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
		
		string names[3] = {"x", "y", "time"};
		
		ABSymbol *pos[3] = {
			new ABSymVarPull("x", 11, updateX),
			new ABSymVarPull("y", 11, updateY),
			new ABSymTime("time", &timer, 0),
		};
		
		vector<string> vTick(names, names+3);
		ABSymbol *tick[1] = {
			new ABSymTick("tick", vTick, &timer, 0.1),
		};
		

		vector<string> v(names, names+2);
		ABSymbol *curve[1] = {
			new ABSymCurve("curve", v, "time", 100, 5),
		};
		
		transform.addSymbols(pos, 3);
		transform.addSymbols(tick, 1);
		transform.addSymbols(curve, 1);
		
		transform.startTick("tick");

	}
	return self;
}

-(CGPoint)getMean {
	
	double buffer[2];
	double time = timer.getTime()-1.0;
	transform.getValues("curve", buffer, time);
	
	float meanX = buffer[0];
	float meanY = buffer[1];
	printf("-> (%.2f, %.2f)\n\n", meanX, meanY);
	return CGPointMake(meanX, meanY);
}




@end


