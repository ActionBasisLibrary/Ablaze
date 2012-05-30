//
//  ABParticleShader.h
//  Ablaze
//
//  Created by Caleb Jordan on 5/29/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef Ablaze_ABParticleShader_h
#define Ablaze_ABParticleShader_h

#include <OpenGLES/ES2/gl.h>
#include <string>

class ABParticleShader {
private:
    unsigned int programId, vertId, fragId;
    
public:
    ABParticleShader(const char *vertPath, const char *fragPath);
    ~ABParticleShader();
    
    void setTransform(float modelviewMatrix[16], float projectionMatrix[16]);
    void engage();
    void disengage();
    
private:
    bool createShaders();
    bool loadAndCompileShaders(const char *vertPath, const char *fragPath);
    bool loadFile(const char *filename, std::string &strout);
};

#endif
