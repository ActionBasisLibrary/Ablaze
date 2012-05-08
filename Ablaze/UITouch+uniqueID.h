//
//  UITouch+uniqueID.h
//  Ablaze
//
//  Created by Matt Rubin on 5/7/12.
//  Based off of https://github.com/nevyn/touchid
//

#import <Foundation/Foundation.h>

@interface UITouch (uniqueID)
// Returns a unique identifier for this particular touch
@property (readonly) NSString* uniqueID;
@end
