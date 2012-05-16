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



bool updateX(float* buffer){
	bool a = [ts getXs:buffer];
	printf("X: %f, %f\n", buffer[0], buffer[1]);
	return a;
}
bool updateY(float* buffer){
	bool a = [ts getYs:buffer];
	printf("Y: %f, %f\n", buffer[0], buffer[1]);
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
			new ABSymPull("touchXs", 2, updateX),
			new ABSymPull("touchYs", 2, updateY),
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
	printf("%f, %f\n\n", meanX, meanY);
	return CGPointMake(meanX, meanY);
}



@end


