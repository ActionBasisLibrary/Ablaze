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
@end
