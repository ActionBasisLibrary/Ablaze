//
//  ABLWrapper.m
//  Ablaze
//
//  Created by Matt Rubin on 5/8/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "ABLWrapper.h"



TouchState* ts;

@implementation ABLWrapper
@synthesize touchState;



bool updateX(float* buffer, unsigned int *count){
	bool a = [ts getXs:buffer withCount:count];
	
	printf("X: ");
	for (int i=0; i<*count; i++) {
		printf("%.1f", buffer[i]);
		if(i<*count-1) printf(", ");
	}
	printf("\n");
	
	return a;
}
bool updateY(float* buffer, unsigned int *count){
	bool a = [ts getYs:buffer withCount:count];
	
	printf("Y: ");
	for (int i=0; i<*count; i++) {
		printf("%.1f", buffer[i]);
		if(i<*count-1) printf(", ");
	}
	printf("\n");
	
	return a;
}

-(void)prepTouchState
{
	ts = touchState;
}

-(id)init {
	if(self = [super init]){
		transform = ABTransform(true);
		
		ABSymbol *pos[2] = {
			new ABSymVarPull("touchXs", 11, updateX),
			new ABSymVarPull("touchYs", 11, updateY),
		};
		
		ABSymbol *mean[2] = {
			new ABSymMean("meanX", "touchXs"),
			new ABSymMean("meanY", "touchYs"),
		};
		
		transform.addSymbols(pos, 2);
		transform.addSymbols(mean, 2);

	}
	return self;
}

-(CGPoint)getMean {
	float meanX = *transform.getValue("meanX");
	float meanY = *transform.getValue("meanY");
	printf("-> (%.2f, %.2f)\n\n", meanX, meanY);
	return CGPointMake(meanX, meanY);
}



@end


