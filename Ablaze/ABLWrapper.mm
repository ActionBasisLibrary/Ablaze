//
//  ABLWrapper.m
//  Ablaze
//
//  Created by Matt Rubin on 5/8/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "ABLWrapper.h"


bool updateX(float* buffer){
	buffer[0] = 100.0;
	buffer[1] = 200.0;
	return true;
}
bool updateY(float* buffer){
	buffer[0] = 300.0;
	buffer[1] = 400.0;
	return true;
}
@implementation ABLWrapper

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

-(void)getMean {
	float meanX = *transform.getValue("meanX");
	float meanY = *transform.getValue("meanY");
	printf("%f, %f\n", meanX, meanY);
}



@end


