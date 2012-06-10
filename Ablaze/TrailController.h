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

@interface TrailController : NSObject <TouchStateDelegate>
{
	ABParticles *particles;
	ABParticleShader *pshader;
	GLKMatrix4 projectionMatrix;
}
@property ABParticles *particles;
@property ABParticleShader *pshader;
@property GLKMatrix4 projectionMatrix;

-(void)update:(NSTimeInterval)timeSinceLastUpdate;
-(void)render;

-(void)touchAdded:(NSString*)key;
-(void)touchUpdated:(NSString*)key;
-(void)touchRemoved:(NSString*)key;

@end
