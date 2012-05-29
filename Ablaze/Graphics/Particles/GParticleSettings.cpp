//
//  GParticleSettings.cpp
//  GParticles
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GParticleSettings.h"


gParticleSettings::gParticleSettings()
{
    functionality = gBasic;
    
    lifeSpan = 4.0;
    birthDelay.set(0.0, 4.0);
    falloff = 0.0;
    
    elasticity = 0.0;
    
    birthPosition.set(gVector3f(0.0), gVector3f(0.0));
    birthPosition.setFunction(gDistribution::gSphere);
    birthVelocity.set(gVector3f(-1.0), gVector3f(1.0));
    birthVelocity.setFunction(gDistribution::gSphere);
    
    zeroSize.set(40.0, 40.0);
    alpha.set(1.0, 0.0);
    alpha.setFunction(gEasing::gCubic);
    
    color.set(gVector3f(1.0), gVector3f(1.0));
    rotationAngle.set(0.0, 0.0);
    rotationAngle.setFunction(gEasing::gLinear);
    
    birthTexturePosition.set(gVector2f(0.5), gVector2f(0.5));
    birthTexturePosition.setFunction(gDistribution::gSquare);
    birthTextureVelocity.set(gVector2f(0.0), gVector2f(0.0));
    birthTextureVelocity.setFunction(gDistribution::gDisk);
    textureSampleRadius.set(0.5, 0.5);
}