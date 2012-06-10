//
//  TrailController.h
//  Ablaze
//
//  Created by Matt Rubin on 6/9/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TouchState.h"
#import "ABParticles.h"
#import "ABParticleShader.h"
#import <GLKit/GLKit.h>
#import "ABLWrapper.h"
#import "Drawable.h"

@interface TrailController : NSObject <TouchStateDelegate, Drawable>
{
	ABParticles *particles;
	ABParticleShader *pshader;
	GLKMatrix4 projectionMatrix;
}
@property ABParticles *particles;
@property ABParticleShader *pshader;
@property GLKMatrix4 projectionMatrix;
@property ABLWrapper *wrapper;

@end
