//
//  GParticleSource.cpp
//  GParticles
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GParticleSource.h"

// Constructors

gParticleSource::gParticleSource()
: capacity(0), windowSize(0.0, 0.0)
{
    init();
}

gParticleSource::gParticleSource(unsigned int capacity)
{
    init();
    
    setCapacity(capacity);
    setWindowSize(256.0, 256.0);
}

gParticleSource::gParticleSource(double windowWidth, double windowHeight, unsigned int capacity)
{
    init();
    
    setCapacity(capacity);
    setWindowSize(windowWidth, windowHeight);
}

// IVars
void gParticleSource::setCapacity(unsigned int num)
{
    capacity = num;
}

unsigned int gParticleSource::getCapacity()
{
    return capacity;
}

void gParticleSource::setFunctionality(gParticleSettings::gFunctionality fLevel)
{
    functionality = fLevel;
}

gParticleSettings::gFunctionality gParticleSource::getFunctionality()
{
    return functionality;
}

void gParticleSource::setSortsPerAdvance(unsigned int num)
{
    sortsPerAdvance = num;
}

unsigned int gParticleSource::getSortsPerAdvance()
{
    return sortsPerAdvance;
}

void gParticleSource::setContinuous(bool flag)
{
    continuous = flag;
}

bool gParticleSource::isContinuous()
{
    return continuous;
}

void gParticleSource::setWindowSize(double width, double height)
{
    windowSize.set(width, height);
    windowPixelSize.set(1.0 / width, 1.0 / height);
}

gVector2f gParticleSource::getWindowSize()
{
    return windowSize;
}

gVector2f gParticleSource::getWindowPixelSize()
{
    return windowPixelSize;
}

void gParticleSource::setBlendMode(gBlendMode mode)
{
    blendMode = mode;
	
	switch (blendMode) {
		case gAdditive: case gAdditiveSort:
			SBlend = GL_SRC_ALPHA;
			DBlend = GL_ONE;
			break;
		case gLinear:
			SBlend = GL_SRC_ALPHA;
			DBlend = GL_ONE_MINUS_SRC_ALPHA;
			break;
		case gMultiplicative:
			SBlend = GL_SRC_COLOR;
			DBlend = GL_ZERO;
			break;
		case gMultiplicativeNeg:
			SBlend = GL_ONE_MINUS_SRC_COLOR;
			DBlend = GL_ZERO;
			break;
			
		default:
			break;
	}
}

gParticleSource::gBlendMode gParticleSource::getBlendMode()
{
    return blendMode;
}

// Private stuff

void gParticleSource::init()
{
    setBlendMode(gAdditive);
    setFunctionality(gParticleSettings::gBasic);
    setContinuous(true);
    setSortsPerAdvance(0);
    
    useForceConstant = useForcePosition = useForceVelocity = false;
}