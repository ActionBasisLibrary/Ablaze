//
//  MultiTrailController.h
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import "ABLWrapper.h"
#import "Drawable.h"

@interface MultiTrailController : NSObject <TouchStateDelegate, Drawable>

@property (assign) GLKMatrix4 projectionMatrix;
@property (strong) ABLWrapper *wrapper;

@end
