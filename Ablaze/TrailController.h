//
//  TrailController.h
//  Ablaze
//
//  Created by Matt Rubin on 6/9/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import "ABLWrapper.h"
#import "Drawable.h"

@interface TrailController : NSObject <TouchStateDelegate, Drawable>

@property (strong) ABLWrapper *wrapper;

@end
