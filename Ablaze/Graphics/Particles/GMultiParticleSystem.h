//
//  GMultiParticleSystem.h
//  GParticles
//
//  Created by Caleb Jordan on 8/18/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include <vector>
#include <list>

#include "GParticleSource.h"
#include "GListParticleSource.h"

#pragma GCC visibility push(default)

class gMultiParticleSystem : public gParticleSource {
private:
    struct gIterator {
        gIndexList::iterator listIterator;
        gParticleSettings::gParticle *pointer;
        
        double distance;
        
        gListParticleSource *source;
        unsigned int texIndex;
        GLenum SBlend, DBlend;
        gParticleShaderSingle *shader;
        
        gIterator(gListParticleSource *aSource);
        double advance();
    private:
        bool started;
    };
    
    struct gSourceReference {
        gListParticleSource *source;
        bool autorelease;
        
        ~gSourceReference() {
            if (autorelease) delete source;
        }
    };
    
    std::vector<gSourceReference> sourceVector;
    
    // Draw stats
    bool orderedDraw, sameSettings;// set by user
    
    std::list<gIterator> cacheList;
    bool cacheSameShader, cacheRebindTexture;
    
public:
    // Constructors
    gMultiParticleSystem();
    
    // Numbers and tricky stuff
    void addNewParticleSource(gListParticleSource *source, bool autorelease = false);
    void removeParticleSource(gListParticleSource *source);
    
    void setOrderedDraw(bool ordered);
    bool getOrderedDraw();
    
    void setSameSettings(bool same);
    bool getSameSettings();
    
    // Actions
    void prepDrawCache();
    
    virtual void advanceBySeconds(double seconds);
    virtual void advanceBySeconds(double seconds, gParticleSource::CollisionCallback callback, void *collPassthru);
    virtual void drawParticles();
    virtual void resetParticles();
    
protected:
    virtual void init();
    
private:
    void drawParticlesOrdered();
    void drawParticlesUnordered();
    
    void drawParticlesSameShader(std::list<gIterator> &list);
    void drawParticlesChangeShader(std::list<gIterator> &list);
};

#pragma GCC visibility pop