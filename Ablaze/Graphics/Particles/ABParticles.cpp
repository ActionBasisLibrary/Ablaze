//
//  ABParticles.cpp
//  Ablaze
//
//  Created by Caleb Jordan on 5/28/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABParticles.h"
#include "GRand.h"

#include <assert.h>
#include <algorithm>

#pragma mark CONSTRUCTORS

ABParticles::ABParticles(int capacity)
: maxCapacity(capacity)
{
    initialize();
}

ABParticles::~ABParticles()
{
    glDeleteVertexArraysOES(1, &vaoId);
    glDeleteBuffers(1, &vboId);
    
    delete[] particleArray;
}

#pragma mark STATE CHANGING METHODS

// Functions for creating particle profiles and accessing them later
const ABParticles::ProfileId ABParticles::createProfile(Profile &profile)
{
    profiles.push_back(profile);
    return (ProfileId)profiles.size()-1;
}

bool ABParticles::setProfile(ProfileId pid, Profile &profile)
{
    if (profiles.size() > pid) {
        profiles[pid] = profile;
        
        return true;
    }
    
    else {
        return false;
    }
}

const ABParticles::Profile &ABParticles::getProfile(ProfileId pid)
{
    assert(pid < profiles.size());
    
    return profiles[pid];
}

#pragma mark ACTION METHODS

static int idxCompare(void *thunk, unsigned int l, unsigned int r)
{
    return (int)r - (int)l;
}

// Functions for particle creation and deletion
bool ABParticles::emitParticles(int numParticles, ABParticles::ProfileId profileId)
{
    if (profileId >= profiles.size())
        return false;

    // TODO: Sort list so that we take the lowest indices
    freeList.sortOddEvenMerge(NULL, idxCompare);

    for (int i = 0; i < numParticles; i++) {
        if (freeList.size() < 1) break;

        unsigned int idx = freeList.popFront();
        profiles[profileId].generateParticle(particleArray+idx);
        particleArray[idx].profile = profileId;
        
        liveList.pushBack(idx);
    }

    return true;
}

// Functions for animation governing updating stats, life, and death
void ABParticles::advanceParticlesBySeconds(double dt)
{
    gIndexList::iterator it = liveList.front();

    while (it.good()) {
        unsigned int idx = it.value();
        Particle *ptr = particleArray + idx;

        // Increment the particle's age
        ptr->age += dt;

        // If particle hasn't been born yet, do nothing
        if (ptr->age < 0.0) {

        }

        // If particle is still in its lifespan, advance it
        else if (ptr->age < profiles[ptr->profile].lifeSpan) {
            // If particle hasn't been born, birth it--see function for details
            if (!ptr->born)
                profiles[ptr->profile].birthParticle(ptr);

            // Now set all changing attributes
            profiles[ptr->profile].updateParticle(ptr, dt);
        }

        // Particle must be past its lifespan, so kill it
        else {
            // If particle profile dictates continuous particles, just recycle this space
            if (profiles[ptr->profile].continuous) {
                profiles[ptr->profile].generateParticle(ptr);
            }

            // Otherwise, kill this particle
            else {
                // IMPORTANT: must be set false, otherwise the system will keep drawing the particle
                ptr->born = 0;

                // Now put the index on the appropriate list
                liveList.remove(it);
                freeList.pushBack(idx);

                // Remove leaves it pointing at next valid place, so back it up to prepare for next()
                it.last();
            }
        }

        it.next();
    }
}

// Engage appropriate attribute arrays
void ABParticles::engage()
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    
    // Enable appropriate arrays
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
//    glEnableVertexAttribArray(vTexId);
    glEnableVertexAttribArray(vSize);
    glEnableVertexAttribArray(vLive);

    size_t stride = (char*)&particleArray[1] - (char*)&particleArray[0];
    
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, false, stride, &particleArray[0].position.x);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, false, stride, &particleArray[0].color);
//    glVertexAttribPointer(vTexId, 1, GL_UNSIGNED_BYTE, false, stride, &particleArray[0].texId);
    glVertexAttribPointer(vSize, 1, GL_FLOAT, false, stride, &particleArray[0].size);
    glVertexAttribPointer(vLive, 1, GL_FLOAT, false, stride, &particleArray[0].born);
    
    engaged = true;
}

// Disengage attribute arrays
void ABParticles::disengage()
{
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);
//    glEnableVertexAttribArray(vTexId);
    glDisableVertexAttribArray(vSize);
    glDisableVertexAttribArray(vLive);
    
    glDepthMask(GL_TRUE);
    
//    glBindVertexArrayOES(0);
    
    engaged = false;
}

// Function for drawing
void ABParticles::renderParticles()
{
    
    if (liveList.size() < 1) return;
    // So we must have particles...

    // TODO: EVERYTHING

    // Prepare vector buffer
    bool wasEngaged = engaged;
    if (!wasEngaged) engage();

    unsigned short *indices = liveList.serializeShort();
    size_t num = liveList.size();

    // Set state appropriately--TODO: Nothing in particular?

    // Call draw methods
    
//    glDrawArrays(GL_POINTS, 0, 24);
    glDrawElements(GL_POINTS, num, GL_UNSIGNED_SHORT, indices);

    // Clean up
    
    delete[] indices;

    if (!wasEngaged) disengage();
}

#pragma mark PRIVATE METHODS

void ABParticles::initialize()
{
    glGenVertexArraysOES(1, &vaoId);
    glGenBuffers(1, &vboId);
    
    particleArray = new Particle[maxCapacity];

    for (int i = 0; i < maxCapacity; i++)
        freeList.pushBack(i);
}

#pragma mark PROFILE METHODS

static void posFnDefault(gVector3f &vect, float dt, const ABParticles::Particle *ptr);
static void velFnDefault(gVector3f &vect, float dt, const ABParticles::Particle *ptr);
static void colFnDefault(gVector4f &vect, float dt, const ABParticles::Particle *ptr);
static void sizeFnDefault(float *size, float dt, const ABParticles::Particle *ptr);

ABParticles::Profile::Profile()
: posFn(posFnDefault), velFn(velFnDefault), accFn(NULL),
startPosFn(NULL), startVelFn(NULL), startAccFn(NULL),
colorFn(colFnDefault), startColorFn(NULL),
sizeFn(NULL), startSizeFn(NULL),
delay(0), lifeSpan(0), continuous(false), texId(-1)
{}

void ABParticles::Profile::generateParticle(Particle *ptr)
{
    ptr->born = 0;
    ptr->age = -randf() * delay;
    ptr->lifeSpan = lifeSpan;
    ptr->texId = texId;
}

void ABParticles::Profile::birthParticle(Particle *ptr)
{
    ptr->born = 1;

    // Set position
    if (startPosFn) startPosFn(ptr->position, 0, ptr);
    else ptr->position = gVector3f(0);

    // Set velocity
    if (startVelFn) startVelFn(ptr->velocity, 0, ptr);
    else ptr->velocity = gVector3f(0);

    // Set acceleration 0
    if (startAccFn) startAccFn(ptr->acceleration, 0, ptr);
    else ptr->acceleration = gVector3f(0);

    // Set color white
    if (startColorFn) startColorFn(ptr->color, 0, ptr);
    else ptr->color = gVector4f(1);

    // Set size to 1.0
    if (startSizeFn) startSizeFn(&ptr->size, 0, ptr);
    else ptr->size = 10.0;
	
	if (initOverrideFn) initOverrideFn(ptr);
}

void ABParticles::Profile::updateParticle(Particle *ptr, double dt)
{
    // Update acceleration
    if (accFn) accFn(ptr->acceleration, dt, ptr);

    // Update velocity from acceleration
    if (velFn) velFn(ptr->velocity, dt, ptr);

    // Update position from velocity
    if (posFn) posFn(ptr->position, dt, ptr);

    // Update color from everything
    if (colorFn) colorFn(ptr->color, dt, ptr);

    // Update size if necessary
    if (sizeFn) sizeFn(&ptr->size, dt, ptr);
}

static void posFnDefault(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
    vect += (gVector3f)ptr->velocity * dt;
}

static void velFnDefault(gVector3f &vect, float dt, const ABParticles::Particle *ptr)
{
    vect += (gVector3f)ptr->acceleration * dt;
}

static void colFnDefault(gVector4f &vect, float dt, const ABParticles::Particle *ptr)
{
    float t = ptr->age / ptr->lifeSpan;
//    vect.a = 0.0;
    vect.a = 4 * sqrtf(.0625 - powf(.5-t, 4));
}

static void sizeFnDefault(float *size, float dt, const ABParticles::Particle *ptr)
{
    float t = ptr->age / ptr->lifeSpan;
    *size = 40.0 * sqrtf(.0625 - powf(.5 - t, 4));
//    *size = 20.0;
}
