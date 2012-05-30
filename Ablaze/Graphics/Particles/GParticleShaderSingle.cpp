//
//  GParticleShaderSingle.cpp
//  GParticles
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GParticleShaderSingle.h"

gParticleShaderSingle::gParticleShaderSingle(gParticleSettings::gFunctionality fLevel)
: functionality(fLevel)
{
    initProgram();
    cacheLocations();
}

void gParticleShaderSingle::setWindowPixelSize(gVector2f windowPixelSize)
{
    pixelSize = windowPixelSize;
}

void gParticleShaderSingle::engage()
{
    gShaderProgram::engage();
    
#ifndef __APPLE__
    glUniform2fARB(locPixelSize, pixelSize.x, pixelSize.y);
#else
    glUniform2f(locPixelSize, pixelSize.x, pixelSize.y);
#endif
}

void gParticleShaderSingle::setParticle(gParticleSettings::gParticle *ptr)
{
#ifndef __APPLE__
    switch (functionality) {
        case gParticleSettings::gAdvanced:
            glUniform2fARB(locTexCoord, ptr->texturePosition.x, ptr->texturePosition.y);
            glUniform1fARB(locSampleRadius, ptr->sampleSize);
            
        case gParticleSettings::gModerate:
            glUniform3fARB(locColor, ptr->color.x, ptr->color.y, ptr->color.z);
            glUniform1fARB(locAngle, ptr->angle);
            
        case gParticleSettings::gBasic:
            glUniform1fARB(locPointSize, ptr->pointSize);
            glUniform1fARB(locAlpha, ptr->alpha);
            break;
            
        default:
            break;
    }
#else
    switch (functionality) {
        case gParticleSettings::gAdvanced:
            glUniform2f(locTexCoord, ptr->texturePosition.x, ptr->texturePosition.y);
            glUniform1f(locSampleRadius, ptr->sampleSize);
            
        case gParticleSettings::gModerate:
            glUniform3f(locColor, ptr->color.x, ptr->color.y, ptr->color.z);
            glUniform1f(locAngle, ptr->angle);
            
        case gParticleSettings::gBasic:
            glUniform1f(locPointSize, ptr->pointSize);
            glUniform1f(locAlpha, ptr->alpha);
            break;
            
        default:
            break;
    }
#endif
}

void gParticleShaderSingle::setSettings(gParticleSettings *settings)
{
#ifndef __APPLE__
    switch (functionality) {
        case gParticleSettings::gAdvanced:
            glUniform1fARB(locFalloff, settings->falloff);
            
        default:
            break;
    }
#else
    switch (functionality) {
        case gParticleSettings::gAdvanced:
            glUniform1f(locFalloff, settings->falloff);
            break;
            
        default:
            break;
    }
#endif
}

void gParticleShaderSingle::initProgram()
{
    #ifndef USE_GLES
//#include "../../External/cmatrix"
#include "../../Graphics/Shaders/apsDraw.vert"
#include "../../Graphics/Shaders/apsDrawAdvanced.geom"
#include "../../Graphics/Shaders/apsDrawModerate.geom"
#include "../../Graphics/Shaders/apsDrawBasic.geom"
#include "../../Graphics/Shaders/apsDrawAdvanced.frag"
#include "../../Graphics/Shaders/apsDrawModerate.frag"
#include "../../Graphics/Shaders/apsDrawBasic.frag"
    
    loadVertexShader(APS_DRAW_VERT);
    
    switch (functionality) {
        case gParticleSettings::gAdvanced:
        {
            loadGeometryShader(APS_DRAW_A_GEOM);
            loadFragmentShader(APS_DRAW_A_FRAG);
        }
            break;
        case gParticleSettings::gModerate:
        {
            loadGeometryShader(APS_DRAW_M_GEOM);
            loadFragmentShader(APS_DRAW_M_FRAG);
        }
            break;
        case gParticleSettings::gBasic:
        {
            loadGeometryShader(APS_DRAW_B_GEOM);
            loadFragmentShader(APS_DRAW_B_FRAG);
        }
            break;
            
        default:
            break;
    }
    
    setProgramiv(GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS);
    setProgramiv(GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP);
    setProgramiv(GL_GEOMETRY_VERTICES_OUT_EXT, 4);
#endif
    
    linkProgram();
    printInfoLog();
}

/*
 GLint locPixelSize, locAlpha, locPointSize, locAngle,
 locPosition, locColor, locTexCoord, locSampleRadius;
 */

void gParticleShaderSingle::cacheLocations()
{
    locPixelSize = getUniformLocation("pixelSize");
    locAlpha = getUniformLocation("inAlpha");
    locPointSize = getUniformLocation("inSize");
    locAngle = getUniformLocation("inAngle");
    locColor = getUniformLocation("inColor");
    locTexCoord = getUniformLocation("inTexCoord");
    locSampleRadius = getUniformLocation("inSampleRadius");
    locFalloff = getUniformLocation("inFalloff");
}