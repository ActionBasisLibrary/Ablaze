//
//  ABParticleShader.cpp
//  Ablaze
//
//  Created by Caleb Jordan on 5/29/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABParticleShader.h"
#include "ABParticles.h"
#include <stdio.h>
#include <fstream>
#include <string>

#pragma mark CONSTRUCTORS

ABParticleShader::ABParticleShader(const char *vertPath, const char *fragPath)
: programId(0), vertId(0), fragId(0)
{
    createShaders();
    
    loadAndCompileShaders(vertPath, fragPath);
}

ABParticleShader::~ABParticleShader()
{
    glDeleteProgram(programId);
    glDeleteShader(vertId);
    glDeleteShader(fragId);
}

#pragma mark ACTIONS

void ABParticleShader::setTransform(float modelviewMatrix[16], float projectionMatrix[16])
{
    int mvloc = glGetUniformLocation(programId, "modelviewMatrix");
    int ploc = glGetUniformLocation(programId, "projectionMatrix");
    
    glUniformMatrix4fv(mvloc, 1, GL_FALSE, modelviewMatrix);
    glUniformMatrix4fv(ploc, 1, GL_FALSE, projectionMatrix);
}

void ABParticleShader::engage()
{
    glUseProgram(programId);
}

void ABParticleShader::disengage()
{
    glUseProgram(0);
}

#pragma mark PRIVATE CREATION METHODS

bool ABParticleShader::createShaders()
{
    // Create program object
    if ((programId = glCreateProgram()) == 0) {
        fprintf(stderr, "Could not create shader program!\n");
        
        return false;
    }
    
    // Create vertex shader
    if ((vertId = glCreateShader(GL_VERTEX_SHADER)) == 0) {
        fprintf(stderr, "Could not create vertex shader!\n");
        
        glDeleteProgram(programId);
        
        programId = 0;
        
        return false;
    }
    
    // Create fragment shader
    if ((fragId = glCreateShader(GL_FRAGMENT_SHADER)) == 0) {
        fprintf(stderr, "Could not create fragment shader!\n");
        
        glDeleteShader(vertId);
        glDeleteProgram(programId);
        
        vertId = 0;
        programId = 0;
        
        return false;
    }
    
    return true;
}

bool ABParticleShader::loadAndCompileShaders(const char *vertPath, const char *fragPath)
{
    int status;
    bool success = true;
    
    std::string vsource, fsource;
    
    // Load files
    bool loadedVert = loadFile(vertPath, vsource);
    bool loadedFrag = loadFile(fragPath, fsource);
    
    const char *VERTEX_SOURCE = vsource.c_str();
    const char *FRAGMENT_SOURCE = fsource.c_str();
    
    if (loadedVert) {
        
        // Try to compile vertex shader
        glShaderSource(vertId, 1, &VERTEX_SOURCE, NULL);
        glCompileShader(vertId);
        
        // If shader could not compile, report the errors
        glGetShaderiv(vertId, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            fprintf(stderr, "Could not compile particle vertex shader.\n");
            
            int length;
            glGetShaderiv(vertId, GL_INFO_LOG_LENGTH, &length);
            
            char buffer[length];
            glGetShaderInfoLog(vertId, length, NULL, buffer);
            
            fprintf(stderr, "%s\n", buffer);
            
            success = false;
        }
        
    }
    
    if (loadedFrag) {
    
        // Try to compile fragment shader
        glShaderSource(fragId, 1, &FRAGMENT_SOURCE, NULL);
        glCompileShader(fragId);
        
        // If shader could not compile, report the errors
        glGetShaderiv(fragId, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            fprintf(stderr, "Could not compile particle fragment shader.\n");
            
            int length;
            glGetShaderiv(fragId, GL_INFO_LOG_LENGTH, &length);
            
            char buffer[length];
            glGetShaderInfoLog(fragId, length, NULL, buffer);
            
            fprintf(stderr, "%s\n", buffer);
            
            success = false;
        }
        
    }
    
    if (loadedVert && loadedFrag) {
        
        // Try to link program
        glAttachShader(programId, vertId);
        glAttachShader(programId, fragId);
//        glLinkProgram(programId);
    
        // Bind all appropriate attributes
        glBindAttribLocation(programId, ABParticles::vPosition, "position");
        glBindAttribLocation(programId, ABParticles::vColor, "color");
//        glBindAttribLocation(programId, ABParticles::vTexId, "texId");
        glBindAttribLocation(programId, ABParticles::vSize, "size");
        glBindAttribLocation(programId, ABParticles::vLive, "live");
        glLinkProgram(programId);
        
        // If program could not link, report the errors
        glGetProgramiv(programId, GL_LINK_STATUS, &status);
        {
            if (status == GL_FALSE)
                fprintf(stderr, "Could not link particle program.\n");
            
            int length;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
            
            char buffer[length];
            glGetProgramInfoLog(programId, length, NULL, buffer);
            
            fprintf(stderr, "%s\n", buffer);
            
            success = false;
        }
        
        return success;
        
    } else {
        
        return false;
        
    }
}

bool ABParticleShader::loadFile(const char *filename, std::string &strout)
{
    std::ifstream file(filename);
    
    if (file) {
        std::getline(file, strout, '\0');
    } else {
        fprintf(stderr, "Could not load file '%s'\n", filename);
        return false;
    }
    
    return true;
}