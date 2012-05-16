//
//  TouchState.m
//  Ablaze
//
//  Created by Matt Rubin on 5/7/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "TouchState.h"
#import "TouchStateTouch.h"
#import "UITouch+uniqueID.h"

@implementation TouchState

- (id) init {
	if(self = [super init]){
		touches = [[NSMutableDictionary alloc] initWithCapacity:11];
		touchOrder = [[NSMutableArray alloc] initWithCapacity:11];
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
				//NSLog(@"%@ %@ )", touchObject.uniqueID, touchObject.phaseString);
				break;
			case UITouchPhaseMoved:
				[self updateTouch:touchObject];
				//
				break;
			case UITouchPhaseStationary:
				//
				break;
			case UITouchPhaseEnded:
			case UITouchPhaseCancelled:
				[self removeTouch:touchObject];
				//NSLog(@"%@ %@ )", touchObject.uniqueID, touchObject.phaseString);
				break;
			default:
				break;
		}
	}
	//[self print];
}

-(void)addTouch:(UITouch *)touch{
	NSString* key = touch.uniqueID;
	TouchStateTouch* tst = [TouchStateTouch touchWithID:key location:[touch locationInView:[touch view]]];
	@synchronized(touches){
		[touches setValue:tst forKey:key];
	}
	@synchronized(touchOrder){
		[touchOrder addObject:key];
	}
}

-(void)updateTouch:(UITouch *)touch{
	NSString* key = touch.uniqueID;
	TouchStateTouch* tst;
	@synchronized(touches){
		tst = [touches valueForKey:key];
		if(tst){
			tst.location = [touch locationInView:[touch view]];
		}
	}
}

-(void)removeTouch:(UITouch *)touch{
	NSString* key = touch.uniqueID;
	@synchronized(touches){
		[touches removeObjectForKey:key];
	}
	@synchronized(touchOrder){
		[touchOrder removeObject:key];
	}
}

-(void)print {
	NSMutableString* out = [NSMutableString stringWithString:@"\nTouches:\n"];
	@synchronized(touches){
		NSEnumerator *e = [touches objectEnumerator];
		TouchStateTouch *tst;
		while ((tst = (TouchStateTouch *)[e nextObject]) ) {
			[out appendFormat:@"%@ = (%.1f, %.1f)\n", tst.uniqueID, tst.location.x, tst.location.y];
		}
	}
	NSLog(@"%@", out);
}


-(BOOL)getXs:(float*)buffer {
	NSString* touchKey;
	BOOL success = YES;
	@synchronized(touchOrder){
		success = ([touchOrder count]>0);
		if(success)
			touchKey = [touchOrder objectAtIndex:0];
	}
	if(!success) return NO;
	@synchronized(touches){
		TouchStateTouch* tst = [touches valueForKey:touchKey];
		success = (tst!=nil);
		if(success){
			buffer[0] = tst.location.x;
			buffer[1] = tst.location.x;
		}
	}
	if(!success) return NO;
	@synchronized(touchOrder){
		success = ([touchOrder count]>1);
		if(success)
			touchKey = [touchOrder objectAtIndex:1];
	}
	if(!success) return YES;
	@synchronized(touches){
		TouchStateTouch* tst = [touches valueForKey:touchKey];
		success = (tst!=nil);
		if(success){
			buffer[1] = tst.location.x;
		}
	}
	return YES;
}

-(BOOL)getYs:(float*)buffer {
	NSString* touchKey;
	BOOL success = YES;
	@synchronized(touchOrder){
		success = ([touchOrder count]>0);
		if(success)
			touchKey = [touchOrder objectAtIndex:0];
	}
	if(!success) return NO;
	@synchronized(touches){
		TouchStateTouch* tst = [touches valueForKey:touchKey];
		success = (tst!=nil);
		if(success){
			buffer[0] = tst.location.y;
			buffer[1] = tst.location.y;
		}
	}
	if(!success) return NO;
	@synchronized(touchOrder){
		success = ([touchOrder count]>1);
		if(success)
			touchKey = [touchOrder objectAtIndex:1];
	}
	if(!success) return YES;
	@synchronized(touches){
		TouchStateTouch* tst = [touches valueForKey:touchKey];
		success = (tst!=nil);
		if(success){
			buffer[1] = tst.location.y;
		}
	}
	return YES;
}

- (NSArray*)touchPoints {
	NSMutableArray *tempPoints = [NSMutableArray new];
	@synchronized(touches){
		NSEnumerator *e = [touches objectEnumerator];
		TouchStateTouch *tst;
		while ((tst = (TouchStateTouch *)[e nextObject]) ) {
			[tempPoints addObject:[NSValue valueWithCGPoint:tst.location]];
		}
	}
	return [NSArray arrayWithArray:tempPoints];
}


@end
