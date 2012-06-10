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

@interface TrailController : NSObject <TouchStateDelegate>
{
	ABParticles *particles;
	ABParticleShader *pshader;
}
@property ABParticles *particles;
@property ABParticleShader *pshader;

-(void)touchAdded:(NSString*)key;
-(void)touchUpdated:(NSString*)key;
-(void)touchRemoved:(NSString*)key;

@end
