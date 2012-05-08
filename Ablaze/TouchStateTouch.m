//
//  TouchStateTouch.m
//  Ablaze
//
//  Created by Matt Rubin on 5/7/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "TouchStateTouch.h"

@implementation TouchStateTouch

@synthesize uniqueID;
@synthesize location;

+(TouchStateTouch*)touchWithID:(NSString*)uID location:(CGPoint)loc {
	TouchStateTouch* touch = [TouchStateTouch new];
	touch.uniqueID = uID;
	touch.location = loc;
	return touch;
}

@end
