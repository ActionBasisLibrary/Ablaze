//
//  TouchTrail.h
//  Ablaze
//
//  Created by Matt Rubin on 6/9/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ABParticles.h"
#import "TouchTrailCallback.h"

@interface TouchTrail : NSObject {
	ABParticles::Profile profile;
	ABParticles::ProfileId profileID;
	TouchTrailCallback *callback;
}
@property (assign) CGPoint position;

- (void)attachToParticleSystem:(ABParticles*)particleSystem;
- (void)detachFromParticleSystem:(ABParticles*)particleSystem;

@end
