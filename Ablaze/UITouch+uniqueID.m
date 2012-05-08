//
//  UITouch+uniqueID.m
//  Ablaze
//
//  Created by Matt Rubin on 5/7/12.
//  Based off of https://github.com/nevyn/touchid
//

#import "UITouch+uniqueID.h"
#import <objc/runtime.h>

static const void *kTCUniqueIdKey = &kTCUniqueIdKey;

@implementation UITouch (uniqueID)
-(NSString*)uniqueID;
{
    id uniq = objc_getAssociatedObject(self, kTCUniqueIdKey);
    if(!uniq) {
        CFUUIDRef uuid = CFUUIDCreate(NULL);
        uniq = (__bridge id)CFUUIDCreateString(NULL, uuid);
        CFRelease(uuid);
        objc_setAssociatedObject(self, kTCUniqueIdKey, uniq, OBJC_ASSOCIATION_RETAIN);
    }
    return uniq;
}

- (NSString*)phaseString {
	switch (self.phase) {
		case UITouchPhaseBegan:
			return @"Began";
			break;
		case UITouchPhaseMoved:
			return @"Moved";
			break;
		case UITouchPhaseStationary:
			return @"Stationary";
			break;
		case UITouchPhaseEnded:
			return @"Ended";
			break;
		case UITouchPhaseCancelled:
			return @"Cancelled";
			break;
		default:
			return nil;
			break;
	}
}

@end
