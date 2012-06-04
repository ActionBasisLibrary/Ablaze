//
//  ABLWrapper.h
//  Ablaze
//
//  Created by Matt Rubin on 5/8/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ABL/ABTransform.h"
#import "TouchState.h"

@interface ABLWrapper : NSObject
{
	ABTransform transform;
	GTimerThread timer;
}
@property (retain) TouchState* touchState;

-(CGPoint)getPosition:(float)secondsAgo;
-(CGPoint)getVelocity:(float)secondsAgo;

@end
