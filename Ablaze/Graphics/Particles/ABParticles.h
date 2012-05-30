//
//  ABParticles.h
//  Ablaze
//
//  Created by Caleb Jordan on 5/28/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef Ablaze_ABParticles_h
#define Ablaze_ABParticles_h

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include <vector>
#include "GIndexList.h"
#include "GVector3f.h"
#include "GVector4f.h"

class ABParticles {
public:
    // Class to hold shared particle data like callbacks
    typedef int ProfileId;
    class Profile;
    
    // Class to hold specific particle data like location
    class Particle;
    
    enum ArrayPositions {
        vPosition   = 1,
        vColor      = 2,
        vTexId      = 3,
        vSize       = 4,
        vLive       = 5,
    };
    
private:
    // The Vertex Array
    unsigned int vaoId, vboId;
    
    // Maximum number of particles this source can support
    const int maxCapacity;
    
    // Array with specific particle data
    Particle *particleArray;
    
    gIndexList freeList, liveList;
    
    // Array of shared particle profiles
    std::vector<Profile> profiles;

    // States necessary to handle attribute pointers
    bool engaged;
    
public:
    // Constructor
    ABParticles(int capacity);
    
    // Destructor
    ~ABParticles();
    
    //// State changing functions
    
    // Functions for creating particle profiles and accessing them later
    const ProfileId createProfile(Profile &profile);
    bool setProfile(ProfileId pid, Profile &profile);
    const Profile &getProfile(ProfileId pid);
    
    // Functions for particle creation and deletion
    bool emitParticles(int numParticles, ProfileId profileId);
    
    // Functions for animation governing updating stats, life, and death
    void advanceParticlesBySeconds(double dt);

    // Engage appropriate attribute arrays
    void engage();

    // Disengage attribute arrays
    void disengage();
    
    // Function for drawing
    void renderParticles();
    
private:
    // Function to initialize all members correctly
    void initialize();
};

class ABParticles::Particle {
public:
    ProfileId profile;
    
    char born, texId;
    float age, size;
    gVector3f position, velocity, acceleration;
    gVector4f color;
    
    char padding[4];
};

class ABParticles::Profile {
public:
    typedef void (*ParamFunction1)(float &val, float dt, const Particle *ptr);
    typedef void (*ParamFunction2)(gVector2f &vect, float dt, const Particle *ptr);
    typedef void (*ParamFunction3)(gVector3f &vect, float dt, const Particle *ptr);
    typedef void (*ParamFunction4)(gVector4f &vect, float dt, const Particle *ptr);
    
    ParamFunction1 sizeFn, startSizeFn;
    ParamFunction3 posFn, velFn, accFn;
    ParamFunction3 startPosFn, startVelFn, startAccFn;
    ParamFunction4 colorFn, startColorFn;
    float delay, lifeSpan;
    char texId;
    bool continuous;
    
private:
    friend class ABParticles;
    
public:
    Profile();
    
private:
    
    // Function to generate a particle at a given location with the right stats
    void generateParticle(Particle *ptr);

    // Function called when particle first becomes visible
    void birthParticle(Particle *ptr);
    
    // Function to advance a given particle correctly
    void updateParticle(Particle *ptr, double dt);
};

#endif
