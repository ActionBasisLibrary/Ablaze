//
//  GListParticleSource.h
//  GParticles
//
//  Created by Caleb Jordan on 8/20/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include "GParticleSource.h"
#include "GIndexList.h"
#include "GParticleShaderSingle.h"

#pragma GCC visibility push(default)

class gListParticleSource : public gParticleSource {
protected:
    friend class gMultiParticleSystem;
    
    gParticleSettings::gParticle *particleArray;
    gIndexList *indexList, *freeIndexList;
    
    double sortTime;
    
private:
    gTexture *texture;
    
public:
    // Constructors
    gListParticleSource(unsigned int capacity);
    gListParticleSource(double windowWidth, double windowHeight, unsigned int capacity);
    ~gListParticleSource();
    
    // IVars
    virtual void setCapacity(unsigned int num);
    void emitParticlesNumbering(unsigned int num);
    
    void setTexture(gTexture *aTexture);
    gTexture *getTexture();
    
    // Actions
    virtual void advanceBySeconds(double seconds);
    virtual void advanceBySeconds(double seconds, gParticleSource::CollisionCallback callback, void *collPassthru);
    virtual void drawParticles();
    virtual void resetParticles();
    virtual void sortParticles(unsigned int steps);
    
protected:
    gListParticleSource();
    
    gParticleShaderSingle *getShader();
    static gParticleShaderSingle *getShader(gParticleSettings::gFunctionality fLevel);
    
    void generateParticleAt(gParticleSettings::gParticle *ptr);
    void advanceParticleAt(gParticleSettings::gParticle *ptr, double seconds);
    
    virtual void init();
    
private:
    void loadShaders();
};

#pragma GCC visibility pop