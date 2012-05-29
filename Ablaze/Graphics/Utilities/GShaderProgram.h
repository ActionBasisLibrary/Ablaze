// GShaderProgram.h
#pragma once

#include <string>

#ifdef USE_GLES
#include <OpenGLES/ES2/gl.h>
#else
#include <OpenGL/OpenGL.h>
#endif

/**
* gShaderProgram - class to use GLSL programs.
* Call loadVertexFile and UploadFragmentShader
* to add shaders to the program. Call engage() to begin
* using the shader and disengage() to stop using it.
* Thanks to Stanford U for teaching me how to use this stuff and the beginning of this class.
*/

#pragma GCC visibility push(default)

class gShaderProgram {
public:
    gShaderProgram();
    ~gShaderProgram();

    //
    //    You need to add at least one vertex shader and one fragment shader. You can add multiple shaders
    //    of each type as long as only one of them has a main() function.
    //
    bool loadVertexShader(const std::string& filename);
    bool loadFragmentShader(const std::string& filename);
	bool loadGeometryShader(const std::string& filename);
    bool loadVertexShader(const char *text);
    bool loadFragmentShader(const char *text);
	bool loadGeometryShader(const char *text);

    void engage();
    void disengage();

    //
    // Set a uniform global parameter of the program by name.
    //
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, float v0, float v1);
    void setUniform(const std::string& name, float v0, float v1, float v2);
    void setUniform(const std::string& name, float v0, float v1, float v2, float v3);
	
	void setUniformi(const std::string& name, int value);
	
	void setUniformv(const std::string& name, int count, float *values);
	void setUniformv(const std::string& name, int count, int *values);
	
	void setUniformMatrix3f(const std::string& name, bool rowMajor, float *matrix);
	void setUniformMatrix4f(const std::string& name, bool rowMajor, float *matrix);
	
	void enableVertexAttributeArray(const std::string& name);
	void disableVertexAttributeArray(const std::string& name);
	void vertexAttributePointer(const std::string& name, GLint size, GLenum type,
								   GLboolean normalized, GLsizei stride, const void *pointer);
	
	void getProgramiv(GLenum pname, GLint *params);
	void setProgramiv(GLenum pname, GLint param);
    
    void linkProgram();
    void printInfoLog();

protected:
    //
    // Helper routine - get the location for a uniform shader parameter.
    //
    GLint getUniformLocation(const std::string& name);
	GLint getAttributeLocation(const std::string& name);

    // OpenGL program object id.
    unsigned int programid;
};

#pragma GCC visibility pop