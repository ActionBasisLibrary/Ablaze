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
    class ProfileCallback;
    
    // Class to hold specific particle data like location
    class Particle;
    
    enum ArrayPositions {
        vPosition   = 1,
        vColor      = 2,
        vTexId      = 3,
        vSize       = 4,
        vLive       = 5,
    };
    
    enum BlendMode {
        gAdditive, gLinear, gMultiplicative, gMultiplicativeNeg, gAdditiveSort
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
    
    // Blend mode
    BlendMode blendMode;
    GLenum SBlend, DBlend;
    
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
    
    bool setNumContinuousParticles(int numParticles, ProfileId profileId);
    
    // Functions for animation governing updating stats, life, and death
    void advanceParticlesBySeconds(double dt);

    // Engage appropriate attribute arrays
    void engage();

    // Disengage attribute arrays
    void disengage();
    
    // Function for drawing
    void renderParticles();
    
    // Settings for blend mode
    void setBlendMode(BlendMode mode);
    BlendMode getBlendMode();
    
private:
    // Function to initialize all members correctly
    void initialize();
};

class ABParticles::Particle {
public:
    friend class ABParticleCallback;
	
    ProfileId profile;
    
    char texId;
    float age, size, lifeSpan, birthTime;
    gVector3f position, velocity, acceleration;
    gVector4f color;
	float born;
    
    char padding[4];
};



class ABParticles::ProfileCallback {
public:
	virtual void initOverride      (ABParticles::Particle *ptr);
	
    virtual void size              (float *val,      float dt, const ABParticles::Particle *ptr);
    virtual void position          (gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	virtual void velocity          (gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	virtual void acceleration      (gVector3f &vect, float dt, const ABParticles::Particle *ptr);
    virtual void color             (gVector4f &vect, float dt, const ABParticles::Particle *ptr);
	
	virtual void startSize         (float *val,      float dt, const ABParticles::Particle *ptr);
    virtual void startPosition     (gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	virtual void startVelocity     (gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	virtual void startAcceleration (gVector3f &vect, float dt, const ABParticles::Particle *ptr);
	virtual void startColor        (gVector4f &vect, float dt, const ABParticles::Particle *ptr);
};


class ABParticles::Profile {
public:
    float delay, lifeSpan;
    char texId;
    bool continuous;
    ProfileCallback *callback;
    
private:
    friend class ABParticles;
    
    int numContinuous, numDelta;
	ProfileCallback defaultCallback;
    
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
