//
//  GParticleSettings.h
//  GParticles
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include "GDataTypes.h"
#include "GInterpolation.h"
#include "GRandom.h"

#pragma GCC visibility push(default)

class gParticleSettings {
public:
    struct gParticle {
        double age, alpha, pointSize, angle;
        gVector3f position, velocity, color, origin;
        
        gVector2f texturePosition, textureVelocity;
        double sampleSize;
        
        bool born;
        
        gParticle() : origin(0.0) {}
    };
    
    enum gFunctionality {
        gBasic, gModerate, gAdvanced
    } functionality;
    
    // Basic Functionality
    
    // Constant for all particles
    double lifeSpan, elasticity;
    gRandomValue1f birthDelay;
    
    // Random for each particle at birth
    gRandomValue3f birthPosition;
    gRandomValue3f birthVelocity;
    
    // Same for all particles, interpolated
    gInterpolatedValue1f zeroSize;
    gInterpolatedValue1f alpha;
    
    // Moderate Functionality
    
    // Same for all particles, interpolated
    gInterpolatedValue3f color;
    gInterpolatedValue1f rotationAngle;
    
    // Advanced Functionality
    
    // Constant
    double falloff;
    
    // Texture manipulation enabled
    gRandomValue2f birthTexturePosition;
    gRandomValue2f birthTextureVelocity;
    gInterpolatedValue1f textureSampleRadius;
    
public:
    gParticleSettings();
};

#pragma GCC visibility pop