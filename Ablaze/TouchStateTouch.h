//
//  TouchStateTouch.h
//  Ablaze
//
//  Created by Matt Rubin on 5/7/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TouchStateTouch : NSObject
{
	NSString* uniqueID;
	CGPoint location;
}
@property (retain) NSString* uniqueID;
@property (assign) CGPoint location;

+(TouchStateTouch*)touchWithID:(NSString*)uID location:(CGPoint)loc;

@end
