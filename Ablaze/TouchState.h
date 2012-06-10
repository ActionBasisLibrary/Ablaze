//
//  TouchState.h
//  Ablaze
//
//  Created by Matt Rubin on 5/7/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>


@protocol TouchStateDelegate <NSObject>
@optional
-(void)touchAdded:(NSString*)key atPoint:(CGPoint)point;
-(void)touchUpdated:(NSString*)key atPoint:(CGPoint)point;
-(void)touchRemoved:(NSString*)key;
@end

#define MAX_TOUCH_COUNT 11

@interface TouchState : NSObject
{
	NSMutableDictionary* touches;
	NSMutableArray* touchOrder;
}
@property (nonatomic, weak) id<TouchStateDelegate> delegate;
@property (readonly) NSArray* touchPoints;

-(void)handleTouches:(NSSet*)changedTouches;

-(BOOL)getXs:(double*)buffer withCount:(unsigned int *)count;
-(BOOL)getYs:(double*)buffer withCount:(unsigned int *)count;

@end
