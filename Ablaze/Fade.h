//
//  Fade.h
//  Ablaze
//
//  Created by Matt Rubin on 6/9/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>

@interface Fade : NSObject
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setup;
- (void)tearDown;

- (void)render;

@end
