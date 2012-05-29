//
//  GParticleShaderSingle.h
//  GParticles
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include "GShaderProgram.h"
#include "GParticleSettings.h"

#pragma GCC visibility push(default)

/*
 struct gParticle {
 double age, alpha, pointSize, angle;
 gVector3f position, velocity, color;
 
 gVector2f texturePosition, textureVelocity;
 double sampleSize;
 };
 */

class gParticleShaderSingle : public gShaderProgram {
private:
    GLint locPixelSize, locAlpha, locPointSize, locAngle, locFalloff,
    locPosition, locColor, locTexCoord, locSampleRadius;
    
    gVector2f pixelSize;
    
    const gParticleSettings::gFunctionality functionality;
    
public:
    // Constructors
    gParticleShaderSingle(gParticleSettings::gFunctionality fLevel);
    
    // iVars
    void setWindowPixelSize(gVector2f windowPixelSize);
    
    // Actions
    void engage();
    void setParticle(gParticleSettings::gParticle *ptr);
    void setSettings(gParticleSettings *settings);
    
private:
    void initProgram();
    void cacheLocations();
};

#pragma GCC visibility pop