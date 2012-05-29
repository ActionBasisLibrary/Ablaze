/*
 *  GTexture.cpp
 *  CS248-Final-Project
 *
 *  Created by Caleb Jordan on 3/9/11.
 *  Copyright 2011 Stanford University. All rights reserved.
 *
 */

#include "GTexture.h"

#ifdef USE_GLES
#define     GL_RGB32F_ARB   GL_RGB16F_EXT
#define     GL_RGBA32F_ARB  GL_RGBA16F_EXT
#define     GL_RGBA8        GL_RGBA8_OES
#endif

gTexture::gTexture()
: mWidth(-1)
, mHeight(-1)
{
    initialize();
}

gTexture::gTexture(int width, int height, gTexture::gImageDefaults aType)
: mWidth(width)
, mHeight(height)
{
	initialize();
	allocateSpace(aType, width, height);
}

void gTexture::initialize()
{
    mTexId = 0;
    glGenTextures(1, &mTexId);
	
	mTexSlot = GL_TEXTURE0;
	
    setFilter(GL_LINEAR, GL_LINEAR);
    setWrap(GL_REPEAT, GL_REPEAT);
}

gTexture::~gTexture()
{
    glDeleteTextures(1, &mTexId);
}
/*
void gTexture::allocate(gImageDefaults aType, int width, int height, const void *data, bool mipMaps)
{
    mWidth = width;
    mHeight = height;
    allocate(aType, data, mipMaps);
}

void gTexture::allocate(gTexture::gImageDefaults aType, const void *data, bool mipMaps)
{
	switch (aType) {
		case gDepth:
			allocate(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, data, mipMaps);
			break;
		case gData:
			allocate(GL_RGB32F_ARB, GL_RGB, GL_FLOAT, data, mipMaps);
			break;
		case gData4:
			allocate(GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT, data, mipMaps);
            break;

		default:
			allocate(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, data, mipMaps);
			break;
	}
}

void gTexture::allocate(GLint internalFormat, GLint format, GLint type, const void *data, bool mipMaps)
{
	engage();
	
	mFormat = format;
	mType = type;
	
    if (data && mipMaps) {
        //glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, mFormat, mType, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
        gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, mWidth, mHeight, mFormat, mType, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, mFormat, mType, data);
    }
	
	disengage();
}
*/
void gTexture::allocateSpace(gImageDefaults aType, int width, int height)
{
    switch (aType) {
		case gDepth:
            allocateSpace(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, width, height);
			break;
		case gData:
            allocateSpace(GL_RGB32F_ARB, GL_RGB, GL_FLOAT, width, height);
			break;
		case gData4:
            allocateSpace(GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT, width, height);
            break;
            
		case gDefault: default:
            allocateSpace(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, width, height);
			break;
	}
}

void gTexture::allocateSpace(GLint internalFormat, GLint format, GLint type, int width, int height)
{
    engage();
    
    mWidth = width;
    mHeight = height;
	
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL);
	
	disengage();
}
    
void gTexture::loadData(gImageDefaults aType, int width, int height, const void *data, bool mipmaps)
{
    switch (aType) {
		case gDepth:
            loadData(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, width, height, data, mipmaps);
			break;
		case gData:
            loadData(GL_RGB32F_ARB, GL_RGB, GL_FLOAT, width, height, data, mipmaps);
			break;
		case gData4:
            loadData(GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT, width, height, data, mipmaps);
            break;
            
		case gDefault: default:
            loadData(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, width, height, data, mipmaps);
			break;
	}
}

void gTexture::loadData(GLint internalFormat, GLint format, GLint type, int width, int height, const void *data, bool mipmaps)
{
    engage();
    
    mWidth = width;
    mHeight = height;
    
    size_t lineSize = width * gTexture::getPixelSize(format, type);
    
    GLint alignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    while ((lineSize & (alignment / 2)) != 0) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, alignment / 2);
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    }
	
    if (data && mipmaps) {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    }
	
	disengage();
}

#ifndef USE_GLES

void *gTexture::getData(GLint format, GLint type)
{
    size_t lineSize = mWidth * gTexture::getPixelSize(format, type);
    
    GLint alignment;
    glGetIntegerv(GL_PACK_ALIGNMENT, &alignment);
    while ((lineSize & (alignment / 2)) != 0) {
        glPixelStorei(GL_PACK_ALIGNMENT, alignment / 2);
        glGetIntegerv(GL_PACK_ALIGNMENT, &alignment);
    }
    
    void *data = malloc(lineSize * mHeight);
    
    engage();
    
    glGetTexImage(GL_TEXTURE_2D, 0, format, type, data);
    
    disengage();
    
    return data;
}

#endif

// Bind this texture for use in subsequent OpenGL drawing.
void gTexture::engage()
{
	glActiveTexture(mTexSlot);
    glBindTexture(GL_TEXTURE_2D, mTexId);
    lastTexSlot = mTexSlot;
}

void gTexture::engage(GLenum slot)
{
	glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, mTexId);
    lastTexSlot = slot;
}

// Un-bind this texture and return to untextured drawing.
void gTexture::disengage()
{
	glActiveTexture(lastTexSlot);
    glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}

unsigned int gTexture::getNumPixels()
{
	return mWidth * mHeight;
}

void gTexture::setTextureSlot(GLenum aSlot)
{
	mTexSlot = aSlot;
}

// Set the OpenGL texture-filtering mode to use for texture
// magnification and minification respectively.
void gTexture::setFilter(GLint magFilter, GLint minFilter)
{
    engage();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    disengage();
}

// Set the OpenGL mode to use for texture addressing in
// the S and T dimensions respectively.
void gTexture::setWrap(GLint wrapS, GLint wrapT)
{
    engage();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    disengage();
}

#ifndef USE_GLES
void gTexture::drawTextureToScreen()
{
    GLboolean lighton, textureon;
    
    glGetBooleanv(GL_LIGHTING, &lighton);
    glGetBooleanv(GL_TEXTURE_2D, &textureon);
    
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    {
        glTexCoord3f(0.0, 0.0, -1.0);
        glVertex2f(-1.0, -1.0);
        
        glTexCoord3f(1.0, 0.0, -1.0);
        glVertex2f(1.0, -1.0);
        
        glTexCoord3f(1.0, 1.0, -1.0);
        glVertex2f(1.0, 1.0);
        
        glTexCoord3f(0.0, 1.0, -1.0);
        glVertex2f(-1.0, 1.0);
    }
    glEnd();
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    if (lighton) glEnable(GL_LIGHTING);
    if (!textureon) glDisable(GL_TEXTURE_2D);
}
#endif

size_t gTexture::getPixelSize(GLint format, GLint type)
{
    size_t numComps, sizeComps;
    switch (format) {
        case GL_RGBA: default:
            numComps = 4;
            break;
        case GL_RGB:
            numComps = 3;
            break;
        case GL_DEPTH_COMPONENT:
            numComps = 1;
            break;
    }
    
    switch (type) {
        case GL_FLOAT:
            sizeComps = 4;
            break;
            
        case GL_UNSIGNED_BYTE:
            sizeComps = 1;
            break;
            
        default:
            break;
    }
    
    return numComps * sizeComps;
}