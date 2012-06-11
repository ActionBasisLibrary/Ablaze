//
//  Kaleidoscope.h
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Drawable.h"
#import <GLKit/GLKit.h>
#import "TouchState.h"

@interface Kaleidoscope : NSObject <Drawable>

@property (assign) GLKMatrix4 projectionMatrix;
@property (strong) TouchState *touchState;

@end
