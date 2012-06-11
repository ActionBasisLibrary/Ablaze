//
//  FountainController.h
//  Ablaze
//
//  Created by Caleb Jordan on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TouchState.h"
#import "ABParticles.h"
#import "ABParticleShader.h"
#import <GLKit/GLKit.h>
#import "Drawable.h"

#import "ABL/ABTransform.h"
#import "ABL/GTimerThread.h"

@interface FountainController : NSObject <TouchStateDelegate, Drawable>
{
	ABParticles *particles;
	ABParticleShader *pshader;
	GLKMatrix4 projectionMatrix;
    ABTransform transform;
    GTimerThread timer;
}
@property (readonly) ABParticles *particles;
@property (readonly) ABParticleShader *pshader;
@property GLKMatrix4 projectionMatrix;

@end
