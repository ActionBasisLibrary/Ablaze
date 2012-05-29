/*
 *  GTexture.h
 *  CS248-Final-Project
 *
 *  Created by Caleb Jordan on 3/9/11.
 *  Copyright 2011 Stanford University. All rights reserved.
 *
 *  Based off of the STTexture class from STLib.
 *
 */

#pragma once

#include <memory>

#ifdef USE_GLES
#include <OpenGlES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#endif
//#include <GLKit/GLKit.h>

#pragma GCC visibility push(default)

class gTexture
{
private:
    GLuint mTexId;
	GLenum mTexSlot, lastTexSlot;
	
	GLenum mFormat, mType;
	
    int mWidth;
    int mHeight;
	
public:
	enum gImageDefaults {
		gDefault, gDepth, gData, gData4
	};
	
    gTexture();
	
	gTexture(int width, int height, gImageDefaults aType = gDefault);
	
    ~gTexture();
	
    /*void allocate(gImageDefaults aType, int width, int height, const void *data, bool mipmaps = false);
	void allocate(gImageDefaults aType, const void *data, bool mipmaps = false);
    void allocate(GLint internalFormat, GLint format, GLint type, const void *data, bool mipmaps = false);*/
    
    void allocateSpace(gImageDefaults aType, int width, int height);
    void allocateSpace(GLint internalFormat, GLint format, GLint type, int width, int height);
    
    void loadData(gImageDefaults aType, int width, int height, const void *data, bool mipmaps = false);
    void loadData(GLint internalFormat, GLint format, GLint type, int width, int height, const void *data, bool mipmaps = false);
    
    void *getData(GLint format, GLint type);
	
	// Actions
	
    void engage();
    void engage(GLenum slot);
    void disengage();
	
	// Copy info

	unsigned int getNumPixels();
	
	// Details
	
	void setTextureSlot(GLenum aSlot);
	
    void setFilter(GLint magFilter, GLint minFilter);
	
    void setWrap(GLint wrapS, GLint wrapT);
	
    int getWidth() const { return mWidth; }
	
    int getHeight() const { return mHeight; }
	
	GLuint getID() { return mTexId; }
    
    static size_t getPixelSize(GLint format, GLint type);
    
    /*
     Following are functions that cannot be done with ES until I figure out how
     */
    
#ifndef USE_GLES
    static void drawTextureToScreen();
	
	void *getData();
#endif
	
private:
    void initialize();

};

#pragma GCC visibility pop
