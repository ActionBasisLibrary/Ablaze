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

    int i;
    for (i = 0; i < numParticles; i++) {
        if (freeList.size() < 1) break;

        unsigned int idx = freeList.popFront();
        profiles[profileId].generateParticle(particleArray+idx);
        particleArray[idx].profile = profileId;
        
        liveList.pushBack(idx);
    }
    
    profiles[profileId].numDelta += i;
    if (profiles[profileId].continuous)
        profiles[profileId].numContinuous += i;

    return true;
}

bool ABParticles::setNumContinuousParticles(int numParticles, ProfileId profileId)
{
    if (profileId >= profiles.size())
        return false;
    
    profiles[profileId].continuous = true;
    
    int difference = numParticles - profiles[profileId].numContinuous;
    
    if (difference > 0) {
        emitParticles(difference, profileId);
    }
    
    else if (difference < 0) {
        profiles[profileId].numDelta += difference;
    } 
    
    profiles[profileId].numContinuous = numParticles;
    
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
            // Don't birth particle if we need to get rid of them
            if (profiles[ptr->profile].numDelta < 1) {
                liveList.remove(it);
                freeList.pushBack(idx);
            }
            
            it.next();
        }

        // If particle is still in its lifespan, advance it
        else if (ptr->age < profiles[ptr->profile].lifeSpan) {
            // If particle hasn't been born, birth it--see function for details
            if (!ptr->born) {
                profiles[ptr->profile].birthParticle(ptr);
                profiles[ptr->profile].numDelta--;
            }

            // Otherwise just update all changing attributes
            profiles[ptr->profile].updateParticle(ptr, dt);
            
            it.next();
        }

        // Particle must be past its lifespan, so kill it
        else {
            // If particle profile dictates continuous particles, just recycle this space
            if (profiles[ptr->profile].continuous) {
                profiles[ptr->profile].generateParticle(ptr);
                profiles[ptr->profile].numDelta++;
                
                it.next();
            }

            // Otherwise, kill this particle
            else {
                // IMPORTANT: must be set false, otherwise the system will keep drawing the particle
                ptr->born = 0;

                // Now put the index on the appropriate list
                liveList.remove(it);
                freeList.pushBack(idx);
            }
        }
    }
}

// Engage appropriate attribute arrays
void ABParticles::engage()
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
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
    
    size_t num = liveList.size();
    unsigned short *indices = liveList.serializeShort();

    // Set state appropriately--TODO: Nothing in particular?

    // Call draw methods
    if (num > 0) {
        
        glDrawElements(GL_POINTS, num, GL_UNSIGNED_SHORT, indices);

        // Clean up
        
        delete[] indices;
        
    }

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

ABParticles::Profile::Profile()
: callback(NULL),
delay(0), lifeSpan(0), continuous(false), texId(-1),
numContinuous(0), numDelta(0)
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

	ProfileCallback *useCallback = &defaultCallback;
	if (callback) useCallback = callback;
	
	useCallback->startPosition(ptr->position, 0, ptr);
	useCallback->startVelocity(ptr->velocity, 0, ptr);
	useCallback->startAcceleration(ptr->acceleration, 0, ptr);
	useCallback->startColor(ptr->color, 0, ptr);
	useCallback->startSize(&ptr->size, 0, ptr);
	
	useCallback->initOverride(ptr);
}

void ABParticles::Profile::updateParticle(Particle *ptr, double dt)
{
	ProfileCallback *useCallback = &defaultCallback;
	if (callback) useCallback = callback;

	useCallback->position(ptr->position, dt, ptr);
	useCallback->velocity(ptr->velocity, dt, ptr);
	useCallback->acceleration(ptr->acceleration, dt, ptr);
	useCallback->color(ptr->color, dt, ptr);
	useCallback->size(&ptr->size, dt, ptr);
}

