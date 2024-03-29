//
//  Drawable.h
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <GLKit/GLKit.h>

@protocol Drawable <NSObject>
-(void)setup;
-(void)tearDown;
-(void)update:(NSTimeInterval)timeSinceLastUpdate;
-(void)render;

@optional

@property (assign) GLKMatrix4 projectionMatrix;

@end
