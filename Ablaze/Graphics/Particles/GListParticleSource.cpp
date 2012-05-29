//
//  GListParticleSource.cpp
//  GParticles
//
//  Created by Caleb Jordan on 8/20/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GListParticleSource.h"

static int shaderUsers = 0;
static gParticleShaderSingle *shaderBasic;//(gParticleSettings::gBasic);
static gParticleShaderSingle *shaderModerate;//(gParticleSettings::gModerate);
static gParticleShaderSingle *shaderAdvanced;//(gParticleSettings::gAdvanced);

// Constructors
gListParticleSource::gListParticleSource()
{
    init();
}

gListParticleSource::gListParticleSource(unsigned int capacity)
{
    init();
    setCapacity(capacity);
}

gListParticleSource::gListParticleSource(double windowWidth, double windowHeight, unsigned int capacity)
{
    init();
    setWindowSize(windowWidth, windowHeight);
    
    setCapacity(capacity);
}

gListParticleSource::~gListParticleSource()
{
    free(particleArray);
    delete indexList;
    
    if (--shaderUsers == 0) {
        free(shaderBasic);
        free(shaderModerate);
        free(shaderAdvanced);
    }
}

#pragma mark IVARS

void gListParticleSource::setCapacity(unsigned int num)
{
    if (num == capacity) return;
    
	unsigned int numToCopy = (capacity < num ? capacity : num);
	
	gParticleSettings::gParticle *newArray = (gParticleSettings::gParticle *)malloc(num * sizeof(gParticleSettings::gParticle));
	
	if (particleArray) {
		memcpy(newArray, particleArray, numToCopy * sizeof(gParticleSettings::gParticle));
		free(particleArray);
	}
	
	particleArray = newArray;
    
    if (capacity < num) {
        for (unsigned int i = capacity; i < num; i++) freeIndexList->pushBack(i);
    } else {
        indexList->removeRange(num, capacity);
        freeIndexList->removeRange(num, capacity);
    }
    
    capacity = num;
}

void gListParticleSource::emitParticlesNumbering(unsigned int num)
{
    for (unsigned int i = 0; i < num; i++) {
        if (freeIndexList->size() < 1) break;
        
        unsigned int idx = freeIndexList->popBack();
        generateParticleAt(particleArray + idx);
        indexList->pushBack(idx);
    }
    
    if (sortsPerAdvance > 0 && particleSettings.birthDelay.getMin() < 1.0 / sortsPerAdvance) {
        sortTime = 2.0;
        sortParticles(1);
    }
}

void gListParticleSource::setTexture(gTexture *aTexture)
{
    texture = aTexture;
}

gTexture *gListParticleSource::getTexture()
{
    return texture;
}

#pragma mark ACTIONS

void gListParticleSource::advanceBySeconds(double seconds)
{
    gIndexList::iterator it = indexList->front();
    while (it.good()) {
        unsigned int idx = it.value();
        gParticleSettings::gParticle *ptr = particleArray + idx;
        
        if (ptr->age < 0.0) {
            ptr->age += seconds;
            ptr->origin = position;
            it.next();
            continue;
        }
        
        if (ptr->age > particleSettings.lifeSpan) {
            if (continuous) {
                generateParticleAt(ptr);
                indexList->remove(it);
                indexList->pushBack(idx);
            } else {
                indexList->remove(it);
                freeIndexList->pushBack(idx);
            }
            continue;
        }
        
        advanceParticleAt(ptr, seconds);
        
        it.next();
    }
    
    sortTime += seconds;
    if (sortsPerAdvance > 0) sortParticles(sortsPerAdvance);
}

void gListParticleSource::advanceBySeconds(double seconds, gParticleSource::CollisionCallback callback, void *collPassthru)
{
    gIndexList::iterator it = indexList->front();
    while (it.good()) {
        unsigned int idx = it.value();
        gParticleSettings::gParticle *ptr = particleArray + idx;
        
        if (ptr->age < 0.0) {
            ptr->age += seconds;
            ptr->origin = position;
            it.next();
            continue;
        }
        
        if (ptr->age > particleSettings.lifeSpan) {
            if (continuous) {
                generateParticleAt(ptr);
                indexList->remove(it);
                indexList->pushBack(idx);
            } else {
                indexList->remove(it);
                freeIndexList->pushBack(idx);
            }
            continue;
        }
        
        gVector3f fromWorld = ptr->position + ptr->origin;
        
        advanceParticleAt(ptr, seconds);
        
        gVector3f toWorld = ptr->position + ptr->origin;
        gVector3f velWorld = ptr->velocity;
        
        double EXTEND = 4.0;
        fromWorld = (fromWorld - toWorld) * EXTEND + toWorld;
        
        double fraction;
        gVector3f normal;
        if (callback(fromWorld, toWorld, fraction, normal, collPassthru)) {
            if (fraction >= 1.0 - 1.0 / EXTEND) {
            
                gVector3f rayIncident = (toWorld - fromWorld) * (1.0 - fraction);
                gVector3f rayReflect = rayIncident - normal * 2.0 * rayIncident.dot(normal);
                gVector3f velReflect = velWorld - normal * 2.0 * velWorld.dot(normal);
                
                //ptr->position = fromWorld - ptr->origin;
                ptr->position = (toWorld - fromWorld) * fraction + fromWorld + rayReflect - ptr->origin;
                ptr->velocity = velReflect * (1.0 - particleSettings.elasticity);
                
            }
        }
        
        it.next();
    }
    
    sortTime += seconds;
    if (sortsPerAdvance > 0) sortParticles(sortsPerAdvance);
}

/**
 TODO: IMPLEMENT THE ACTUAL DRAWING WITH ES
 */
void gListParticleSource::drawParticles()
{
    GLboolean mask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &mask);
    switch (blendMode) {
        case gAdditive:
            glDepthMask(false);
            break;
        default:
            glDepthMask(true);
            break;
    }
    
	glEnable(GL_BLEND);
	glBlendFunc(SBlend, DBlend);
    
    //glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    //glTranslatef(position.x, position.y, position.z);
    
    // Draw settings in place
    
    if (texture) texture->engage(GL_TEXTURE0);
    
    getShader()->setWindowPixelSize(getWindowPixelSize());
    getShader()->engage();
    getShader()->setSettings(&particleSettings);
    
    gIndexList::iterator it = indexList->front();
    while (it.good()) {
        gParticleSettings::gParticle *ptr = particleArray + it.value();
        
        if (ptr->age < 0.0) {
            it.next();
            continue;
        }
        
        getShader()->setParticle(ptr);
        gVector3f pos = ptr->position + ptr->origin;
        
#ifndef USE_GLES
        glBegin(GL_POINTS);
        {
			glVertex3fv(&(pos.x));
        }
        glEnd();
#endif
        
        it.next();
    }
    
    getShader()->disengage();
    
    if (texture) texture->disengage();
    
    // Reset draw settings
    
    //glPopMatrix();
	
	glDisable(GL_BLEND);
	glDepthMask(mask);
}

void gListParticleSource::resetParticles()
{
    gIndexList::iterator it = indexList->front();
    
    while (it.good()) {
        generateParticleAt(particleArray + it.value());
        it.next();
    }
}

/*
 * one < two means one should draw first, so distance to eye should be greater
 */

struct thunkStruct {
    gParticleSettings::gParticle *array;
    gVector3f eyeLocation;
};

static int distanceCompare(void *thunkPtr, unsigned int one, unsigned int two)
{
    thunkStruct *thunk = (thunkStruct *)thunkPtr;
    
    gVector3f oneVect = thunk->array[one].position - thunk->eyeLocation;
    gVector3f twoVect = thunk->array[two].position - thunk->eyeLocation;
    double oneDistSq = oneVect.lengthSq();
    double twoDistSq = twoVect.lengthSq();
    
    return oneDistSq > twoDistSq ? 1 : -1;
}

void gListParticleSource::sortParticles(unsigned int steps)
{
    if (sortTime * steps > 1.0) {
        
        thunkStruct thunk;
        thunk.array = particleArray;
        thunk.eyeLocation = cameraPosition - position;
        
        indexList->sortOddEvenMerge(&thunk, distanceCompare);
        
        sortTime = 0.0;
    }
}

#pragma mark PROTECTED

gParticleShaderSingle *gListParticleSource::getShader()
{
    switch (functionality) {
        case gParticleSettings::gAdvanced:
            return shaderAdvanced;
        case gParticleSettings::gModerate:
            return shaderModerate;
        case gParticleSettings::gBasic: default:
            return shaderBasic;
    }
}

gParticleShaderSingle *gListParticleSource::getShader(gParticleSettings::gFunctionality fLevel)
{
    switch (fLevel) {
        case gParticleSettings::gAdvanced:
            return shaderAdvanced;
        case gParticleSettings::gModerate:
            return shaderModerate;
        case gParticleSettings::gBasic: default:
            return shaderBasic;
    }
}

#pragma mark PRIVATE

void gListParticleSource::generateParticleAt(gParticleSettings::gParticle *ptr)
{
    switch (functionality) {
        case gParticleSettings::gAdvanced:
            ptr->texturePosition = particleSettings.birthTexturePosition.value();
            ptr->textureVelocity = particleSettings.birthTextureVelocity.value();
            
        case gParticleSettings::gModerate:
            
        case gParticleSettings::gBasic:
            ptr->position = particleSettings.birthPosition.value();
            ptr->velocity = particleSettings.birthVelocity.value();
            ptr->age = -particleSettings.birthDelay.value();
            ptr->origin = position;
            
        default:
            break;
    }
}

void gListParticleSource::advanceParticleAt(gParticleSettings::gParticle *ptr, double seconds)
{
    ptr->age += seconds;
    double ntime = ptr->age / particleSettings.lifeSpan;
    
    switch (functionality) {
        case gParticleSettings::gAdvanced:
            ptr->texturePosition += ptr->textureVelocity * seconds;
            ptr->sampleSize = particleSettings.textureSampleRadius.valueAt(ntime);
            
        case gParticleSettings::gModerate:
            ptr->color = particleSettings.color.valueAt(ntime);
            ptr->angle = particleSettings.rotationAngle.valueAt(ntime);
            
        case gParticleSettings::gBasic:
        {
            gVector3f dv;
            if (useForceConstant) dv += forceConstant;
            if (useForcePosition) dv += forcePosition * ptr->position;
            if (useForceVelocity) dv += forceVelocity * ptr->velocity;
            
            ptr->position += ptr->velocity * seconds;
            ptr->velocity += dv * seconds;
            
            ptr->pointSize = particleSettings.zeroSize.valueAt(ntime);
            ptr->alpha = particleSettings.alpha.valueAt(ntime);
        }
            
        default:
            break;
    }
}

// Static shader stuff

void gListParticleSource::init()
{
    gParticleSource::init();
    
    particleArray = NULL;
    texture = NULL;
    
    indexList = new gIndexList();
    freeIndexList = new gIndexList();
    
    loadShaders();
}

void gListParticleSource::loadShaders()
{
    if (shaderUsers++ == 0) {
        shaderBasic = new gParticleShaderSingle(gParticleSettings::gBasic);
        shaderModerate = new gParticleShaderSingle(gParticleSettings::gModerate);
        shaderAdvanced = new gParticleShaderSingle(gParticleSettings::gAdvanced);
    }
}