//
//  TouchState.m
//  Ablaze
//
//  Created by Matt Rubin on 5/7/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "TouchState.h"
#import "UITouch+uniqueID.h"

@implementation TouchState

- (id) init {
	if(self = [super init]){
		touches = [[NSMutableSet alloc] init];
	}
	return self;
}

-(void)handleTouches:(NSSet*)changedTouches {
	NSEnumerator *e = [changedTouches objectEnumerator];
	UITouch *touchObject;
	while ( (touchObject = (UITouch *)[e nextObject]) ) {
		//NSLog(@"%f: %@ %@ (%u)",touchObject.timestamp,  touchObject.uniqueID, touchObject.phaseString, touchObject.tapCount);
		switch (touchObject.phase) {
			case UITouchPhaseBegan:
				[self addTouch:touchObject];
				break;
			case UITouchPhaseMoved:
				//
				break;
			case UITouchPhaseStationary:
				//
				break;
			case UITouchPhaseEnded:
			case UITouchPhaseCancelled:
				[self removeTouch:touchObject];
				break;
			default:
				break;
		}
	}
}

-(void)addTouch:(UITouch *)touch{
	@synchronized(touches){
		[touches addObject:touch.uniqueID];
	}
}

-(void)removeTouch:(UITouch *)touch{
	@synchronized(touches){
		[touches removeObject:touch.uniqueID];
	}
}

-(void)print {
	@synchronized(touches){
		NSLog(@"\n%@\n", touches);
	}
}

@end
