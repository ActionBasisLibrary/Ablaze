//
//  GMultiParticleSystem.cpp
//  GParticles
//
//  Created by Caleb Jordan on 8/18/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GMultiParticleSystem.h"

#ifdef USE_GLES
#define GL_MAX_TEXTURE_UNITS GL_MAX_TEXTURE_IMAGE_UNITS
#endif

// Constructors
gMultiParticleSystem::gMultiParticleSystem()
{
    init();
}

// Numbers and tricky stuff
void gMultiParticleSystem::addNewParticleSource(gListParticleSource *source, bool autorelease)
{
    gSourceReference ref;
    ref.source = source;
    ref.autorelease = autorelease;
    
    sourceVector.push_back(ref);
}

void gMultiParticleSystem::removeParticleSource(gListParticleSource *source)
{
    std::vector<gSourceReference>::iterator it = sourceVector.begin();
    
    while (it != sourceVector.end()) {
        if (it->source == source) {
            sourceVector.erase(it);
            return;
        }
        it++;
    }
}

void gMultiParticleSystem::setOrderedDraw(bool ordered)
{
    orderedDraw = ordered;
}

bool gMultiParticleSystem::getOrderedDraw()
{
    return orderedDraw;
}

void gMultiParticleSystem::setSameSettings(bool same)
{
    sameSettings = same;
}

bool gMultiParticleSystem::getSameSettings()
{
    return sameSettings;
}

// Actions
void gMultiParticleSystem::prepDrawCache()
{
    cacheRebindTexture = false;
    cacheSameShader = true;
    
    unsigned int maxTexId = 0, numTexUnits = 0;
    
    cacheList.clear();
    
    // Fill cacheList with iterators with source vars
    
    std::vector<gSourceReference>::iterator sourceIt = sourceVector.begin();
    gParticleShaderSingle *firstShader = sourceIt->source->getShader();

    while (sourceIt != sourceVector.end()) {
        gIterator it((sourceIt++)->source);
        
        // Set sameShader false if more than one shader is present
        if (firstShader != it.shader) cacheSameShader = false;
        
        // Just find highest texture id so we can use them as indexes
        if (maxTexId < it.texIndex) maxTexId = it.texIndex;
        
        cacheList.push_back(it);
    }
    
    // Create array to store texture unit of various texture indexes
    int array[maxTexId + 1];
    for (unsigned int i = 0; i < maxTexId + 1; i++) {
        array[i] = -1;
    }
    
    std::list<gIterator>::iterator listIt = cacheList.begin();
    while (listIt != cacheList.end()) {
        unsigned int idx = listIt->texIndex;
        if (idx != 0 && array[idx] == -1) {
            if (numTexUnits == GL_MAX_TEXTURE_UNITS) {
                // Too many textures--we'll just switch each time (SUCK)
                cacheRebindTexture = true;
                break;
            }
            array[idx] = numTexUnits++;
        }
        listIt++;
    }
    
    // Not too many--just store texture unit instead of texture id and we'll prebind
    if (!cacheRebindTexture) {
        listIt = cacheList.begin();
        while (listIt != cacheList.end()) {
            listIt->texIndex = array[listIt->texIndex];
            listIt++;
        }
    }
}

void gMultiParticleSystem::advanceBySeconds(double seconds)
{
    std::vector<gSourceReference>::iterator it = sourceVector.begin();
    
    while (it != sourceVector.end()) {
        it->source->cameraPosition = cameraPosition;
        it->source->advanceBySeconds(seconds);
        it++;
    }
}

void gMultiParticleSystem::advanceBySeconds(double seconds, gParticleSource::CollisionCallback callback, void *collPassthru)
{
    std::vector<gSourceReference>::iterator it = sourceVector.begin();
    
    while (it != sourceVector.end()) {
        it->source->cameraPosition = cameraPosition;
        it->source->advanceBySeconds(seconds, callback, collPassthru);
        it++;
    }
}

void gMultiParticleSystem::drawParticles()
{
    if (orderedDraw && sourceVector.size() > 1) drawParticlesOrdered();
    else drawParticlesUnordered();
}

void gMultiParticleSystem::resetParticles()
{
    std::vector<gSourceReference>::iterator it = sourceVector.begin();
    
    while (it != sourceVector.end()) {
        it->source->resetParticles();
        it++;
    }
}

// Protected stuff
void gMultiParticleSystem::init()
{
    gParticleSource::init();
    
    orderedDraw = false;
    sameSettings = true;
    
    cacheRebindTexture = false;
    cacheSameShader = true;
}

// Private stuff GAH

void gMultiParticleSystem::drawParticlesOrdered()
{
        
    GLboolean mask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &mask);
    glDepthMask(true);
    
    glEnable(GL_BLEND);
    
    // Setup what we can for all shaders
    
    {
        gVector2f wps = getWindowPixelSize();
        gListParticleSource::getShader(gParticleSettings::gBasic)->setWindowPixelSize(wps);
        gListParticleSource::getShader(gParticleSettings::gModerate)->setWindowPixelSize(wps);
        gListParticleSource::getShader(gParticleSettings::gAdvanced)->setWindowPixelSize(wps);
    }
    
    // Build initial draw list from cached seeds, sorted with farthest first
    
    std::list<gIterator> drawList;
    std::list<gIterator>::iterator drawIt;
    
    {
        std::list<gIterator>::iterator cacheIt = cacheList.begin();
        
        while (cacheIt != cacheList.end()) {
            gIterator it = *(cacheIt++);
            if (it.advance() < 0.0) continue;
            
            drawIt = drawList.begin();
            while (drawIt != drawList.end()) {
                if (it.distance > drawIt->distance) break;
                drawIt++;
            }
            
            drawList.insert(drawIt, it);
        }
    }
    
    std::list<gIterator> drawListCopy(drawList);
    
    // If we have few textures, prebind them to different texture units and switch with shader var
    
    if (!cacheRebindTexture) {
        drawIt = drawList.begin();
        while (drawIt != drawList.end()) {
            drawIt->source->getTexture()->engage(GL_TEXTURE0 + drawIt->texIndex);
            drawIt++;
        }
    }
    
    // Set initial shader and texture--init shader necessary if cacheSameShader is true 
    
    gParticleShaderSingle *currentShader = drawList.front().shader;
    currentShader->engage();
    currentShader->setSettings(&drawList.front().source->particleSettings);
    
    int currentTexId = -1;
    
    // This loops until all iterators have reached liveCount of their sources
    
    bool set = false;
    while (drawList.size() > 0) {
        gIterator it = drawList.front();
        drawList.pop_front();
        gParticleSettings::gParticle *ptr = it.pointer;
        
        // Don't do the hard work if particle isn't alive yet
        if (ptr->age >= 0.0) {
        
            if (!set) {
                // Obvious stuff
                glBlendFunc(it.SBlend, it.DBlend);
                
                // Switch shaders if necessary
                if (!cacheSameShader && (it.shader != currentShader)) {
                    currentShader = it.shader;
                    currentShader->engage();
                }
                
                // Reset settings if this flag is not set
                if (!sameSettings) {
                    currentShader->setSettings(&it.source->particleSettings);
                }
                
                // Switch texture if necessary
                if (it.texIndex != currentTexId) {
                    currentTexId = it.texIndex;
                    if (cacheRebindTexture) {
                        glBindTexture(GL_TEXTURE_2D, currentTexId);
                    } else {
                        currentShader->setUniformi("texture", currentTexId);
                    }
                }
                set = true;
            }
            
            // GO GO!
            currentShader->setParticle(ptr);
            
//            glBegin(GL_POINTS);
//            {
//                glVertex3fv(&(ptr->position.x));
//            }
//            glEnd();
            
        }
        
        // Advance iterator, and if it's not at end re-sort it into array
        if (it.advance() >= 0.0) {
            drawIt = drawList.begin();
            while (drawIt != drawList.end()) {
                if (it.distance > drawIt->distance) break;
                drawIt++; set = false;
            }
            drawList.insert(drawIt, it);
        } else {
            set = false;
        }
    }
    
    // Done drawing, put it all back
    
    currentShader->disengage();
    
    if (cacheRebindTexture) {
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        drawIt = drawListCopy.begin();
        while (drawIt != drawListCopy.end()) {
            drawIt->source->getTexture()->disengage();
            drawIt++;
        }
    }
    
	glDisable(GL_BLEND);
    glDepthMask(mask);
}

void gMultiParticleSystem::drawParticlesUnordered()
{
    std::vector<gSourceReference>::iterator it = sourceVector.begin();
    
    gVector2f ws = getWindowSize();
    while (it != sourceVector.end()) {
        it->source->setWindowSize(ws.x, ws.y);
        it->source->drawParticles();
        it++;
    }
}

// Support

gMultiParticleSystem::gIterator::gIterator(gListParticleSource *aSource)
: source(aSource)
{
    started = false;
    distance = -1.0;
    listIterator = aSource->indexList->end();
    
    shader = source->getShader();
    SBlend = source->SBlend;
    DBlend = source->DBlend;
    if (source->getTexture()) texIndex = source->getTexture()->getID();
    else texIndex = 0;
}

double gMultiParticleSystem::gIterator::advance()
{
    if (started) {
        listIterator.next();
    } else {
        started = true;
        listIterator = source->indexList->front();
    }
    
    if (listIterator.good()) {
        pointer = source->particleArray + listIterator.value();
        gVector3f vect = source->cameraPosition - (source->position + pointer->position);
        distance = vect.lengthSq();
    } else {
        pointer = NULL;
        distance = -1.0;
    }
    return distance;
}