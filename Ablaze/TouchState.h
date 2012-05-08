//
//  TouchState.h
//  Ablaze
//
//  Created by Matt Rubin on 5/7/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TouchState : NSObject
{
	NSMutableDictionary* touches;
}

-(void)handleTouches:(NSSet*)changedTouches;
-(void)print;

@end
