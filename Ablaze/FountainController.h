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
#import "FountainCallback.h"

#import "ABL/ABTransform.h"
#import "ABL/GTimerThread.h"

@interface FountainController : NSObject <TouchStateDelegate, Drawable>
{
    GLKMatrix4 projectionMatrix;
    GTimerThread timer;
    
	ABParticles *particles;
	ABParticleShader *pshader;
    
    FountainCallback callback;
    ABParticles::ProfileId pid0;
    
    ABTransform transform;
}
@property (readonly) ABParticles *particles;
@property (readonly) ABParticleShader *pshader;
@property GLKMatrix4 projectionMatrix;
@property (retain) TouchState* touchState;

@end