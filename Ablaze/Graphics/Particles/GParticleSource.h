//
//  GParticleSource.h
//  GParticles
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include <memory.h>
#include "GTexture.h"

#include "GParticleSettings.h"

#pragma GCC visibility push(default)

class gParticleSource
{
public:
    enum gBlendMode {
        gAdditive, gLinear, gMultiplicative, gMultiplicativeNeg, gAdditiveSort
    };
    
    typedef bool CollisionCallback(gVector3f from, gVector3f to, double &fraction, gVector3f &normal, void *passthru);
    
    // Particle generation packet
    gParticleSettings particleSettings;
    
    // Variable of source itself
    gVector3f position;
    gVector3f cameraPosition;
    
    // Same for all particles, changed by user
    gVector3f forcePosition;
    gVector3f forceVelocity;
    gVector3f forceConstant;
    
    bool useForceConstant, useForcePosition, useForceVelocity;
    
protected:
    // Variables of source itself
    unsigned int capacity, sortsPerAdvance;
    bool continuous;
    
    gParticleSettings::gFunctionality functionality;
    
    // Drawing variables
    gBlendMode blendMode;
    GLenum SBlend, DBlend;
    
private:
    // Variables from the outside
    gVector2f windowSize;
    gVector2f windowPixelSize;
    
public:
    // Constructors
    //gParticleSource();
    gParticleSource(unsigned int capacity);
    gParticleSource(double windowWidth, double windowHeight, unsigned int capacity = 1024);
    
    // IVars
    virtual void setCapacity(unsigned int num);
    unsigned int getCapacity();
    
    void setSortsPerAdvance(unsigned int num);
    unsigned int getSortsPerAdvance();
    
    void setContinuous(bool flag);
    bool isContinuous();
    
    virtual void setFunctionality(gParticleSettings::gFunctionality fLevel);
    gParticleSettings::gFunctionality getFunctionality();
    
    void setWindowSize(double width, double height);
    gVector2f getWindowSize();
    gVector2f getWindowPixelSize();
    
    void setBlendMode(gBlendMode mode);
    gBlendMode getBlendMode();
    
    // Actions
    virtual void advanceBySeconds(double seconds) = 0;
    virtual void advanceBySeconds(double seconds, gParticleSource::CollisionCallback callback, void *collPassthru) = 0;
    
    virtual void drawParticles() = 0;
    virtual void resetParticles() = 0;
    
protected:
    gParticleSource();
    
    virtual void init();
};

#pragma GCC visibility pop
