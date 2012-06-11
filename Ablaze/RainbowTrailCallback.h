//
//  RainbowTrailCallback.h
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef Ablaze_RainbowTrailCallback_h
#define Ablaze_RainbowTrailCallback_h

#include "ABParticles.h"
#import "ABLWrapper.h"

class RainbowTrailCallback : public ABParticles::ProfileCallback {
public:
	void initOverride(ABParticles::Particle *ptr);
	ABLWrapper *wrapper;
};


#endif
