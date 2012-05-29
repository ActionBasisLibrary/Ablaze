// GShaderProgram.cpp

/* Include-order dependency!
*
* GLEW must be included before the standard GL.h header.
* In this case, it means we must violate the usual design
* principle of always including Foo.h first in Foo.cpp.
*/
#ifdef __APPLE__
#define GLEW_VERSION_2_0 1
#else
#define GLEW_STATIC
#include "GL/glew.h"
#endif

#include "GShaderProgram.h"

#include <assert.h>
#include <string>
#include <fstream>
#include <sstream>

//

gShaderProgram::gShaderProgram()
{
    if(GLEW_VERSION_2_0) {
        programid = glCreateProgram();
    }
#ifndef __APPLE__
    else {
        programid = glCreateProgramObject();
    }
#endif
}

gShaderProgram::~gShaderProgram()
{
    if(GLEW_VERSION_2_0)
        glDeleteProgram(programid);
#ifndef __APPLE__
    else
        glDeleteObject(programid);
#endif    
}

bool gShaderProgram::loadVertexShader(const std::string& filename)
{
    std::ifstream in(filename.c_str());
    if(!in) {
      fprintf(stderr, "Failed to open shader file '%s'\n", filename.c_str());
        //assert(false);
        return false;
    }
    std::stringstream ss;
    ss << in.rdbuf();
    
    std::string str = ss.str();
    const char* ptr = str.c_str();

    return loadVertexShader(ptr);
}

bool gShaderProgram::loadFragmentShader(const std::string& filename)
{
    std::ifstream in(filename.c_str());
    if(!in) {
      fprintf(stderr, "Failed to open shader file '%s'\n", filename.c_str());
        //assert(false);
        return false;
    }
    std::stringstream ss;
    ss << in.rdbuf();
    
    std::string str = ss.str();
    const char* ptr = str.c_str();

    return loadFragmentShader(ptr);
}

bool gShaderProgram::loadGeometryShader(const std::string& filename)
{
    std::ifstream in(filename.c_str());
    if(!in) {
		fprintf(stderr, "Failed to open shader file '%s'\n", filename.c_str());
        //assert(false);
        return false;
    }
    std::stringstream ss;
    ss << in.rdbuf();
    
    std::string str = ss.str();
    const char* ptr = str.c_str();
	
    return loadGeometryShader(ptr);
}

bool gShaderProgram::loadVertexShader(const char *text)
{
    // Buffer for error messages
    static const int kBufferSize = 1024;
    char buffer[1024];
    
    if(GLEW_VERSION_2_0) 
    {
        GLuint shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader, 1, &text, NULL);
        glCompileShader(shader);
        GLint result = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if(result != GL_TRUE) {
            GLsizei length = 0;
            glGetShaderInfoLog(shader, kBufferSize-1,
                               &length, buffer);
            fprintf(stderr, "Vertex Shader: GLSL error\n%s\n", buffer);
            //assert(false);
			return false;
        }
        glAttachShader(programid, shader);
        glLinkProgram(programid);
		return true;
    }
#ifndef __APPLE__
    else
    {
        GLuint shader = glCreateShaderObject(GL_VERTEX_SHADER);
        glShaderSource(shader, 1, &ptr, NULL);
        glCompileShader(shader);
        GLint result = 0;
        glGetObjectParameteriv(shader, GL_OBJECT_COMPILE_STATUS_, &result);
        if(result != GL_TRUE) {
            GLsizei length = 0;
            glGetInfoLog(shader, kBufferSize-1,
                            &length, buffer);
            fprintf(stderr, "Vertex Shader: GLSL error\n%s\n", buffer);
            //assert(false);
			return false;
        }
        glAttachObject(programid, shader);
        glLinkProgram(programid);
		return true;
    }
#endif
}

bool gShaderProgram::loadFragmentShader(const char *text)
{
    // Buffer for error messages
    static const int kBufferSize = 1024;
    char buffer[1024];
    
    if(GLEW_VERSION_2_0)
    {
        GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader, 1, &text, NULL);
        glCompileShader(shader);
        GLint result = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if(result != GL_TRUE) {
            GLsizei length = 0;
            glGetShaderInfoLog(shader, kBufferSize-1,
                               &length, buffer);
            fprintf(stderr, "Fragment Shader: GLSL error\n%s\n", buffer);
            //assert(false);
			return false;
        }
        glAttachShader(programid, shader);
        glLinkProgram(programid);
		return true;
    }
#ifndef __APPLE__
    else
    {
        GLuint shader = glCreateShaderObject(GL_FRAGMENT_SHADER);
        glShaderSource(shader, 1, &ptr, NULL);
        glCompileShader(shader);
        GLint result = 0;
        glGetObjectParameteriv(shader, GL_OBJECT_COMPILE_STATUS_, &result);
        if(result != GL_TRUE) {
            GLsizei length = 0;
            glGetInfoLog(shader, kBufferSize-1,
                            &length, buffer);
            fprintf(stderr, "Fragment Shader: GLSL error\n%s\n", buffer);
            //assert(false);
			return false;
        }
        glAttachObject(programid, shader);
        glLinkProgram(programid);
		return true;
    }
#endif
}

bool gShaderProgram::loadGeometryShader(const char *text)
{
    // Buffer for error messages
    static const int kBufferSize = 1024;
    char buffer[1024];
	
#ifdef USE_GLES
    {
        
        return false;
    }
#else
    {
        GLuint shader = glCreateShader(GL_GEOMETRY_SHADER_EXT);
        glShaderSource(shader, 1, &text, NULL);
        glCompileShader(shader);
        GLint result = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if(result != GL_TRUE) {
            GLsizei length = 0;
            glGetShaderInfoLog(shader, kBufferSize-1,
							   &length, buffer);
            fprintf(stderr, "Geometry Shader: GLSL error\n%s\n", buffer);
            //assert(false);
			return false;
        }
        glAttachShader(programid, shader);
        glLinkProgram(programid);
		return true;
    }
#endif
}

void gShaderProgram::printInfoLog()
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    glGetProgramiv(programid, GL_INFO_LOG_LENGTH,&infologLength);
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(programid, infologLength, &charsWritten, infoLog);
        fprintf(stderr, "GLSL program error\n%s\n", infoLog);
        free(infoLog);
    }else{
	 	printf("Program Info Log: OK\n");
    }
}

/* engage the program as the current program. */
void gShaderProgram::engage() {
    if(GLEW_VERSION_2_0)
        glUseProgram(programid);    
#ifndef __APPLE__
    else
        glUseProgramObject(programid);
#endif
}

/* Un-bind the program. */
void gShaderProgram::disengage() {
    if(GLEW_VERSION_2_0)
        glUseProgram(0);
    else
        glUseProgram(0);
}

// Set a uniform global parameter of the program by name.
void gShaderProgram::setUniform(const std::string& name, float value)
{
    GLint location = getUniformLocation(name);
    if(GLEW_VERSION_2_0) {
        glUniform1f(location, value);
    }
    else {
        glUniform1f(location, value);
    }
}

// Set a uniform global parameter of the program by name.
void gShaderProgram::setUniform(const std::string& name, float v0, float v1)
{
    GLint location = getUniformLocation(name);
    if(GLEW_VERSION_2_0) {
        glUniform2f(location, v0, v1);
    }
    else {
        glUniform2f(location, v0, v1);
    }
}

// Set a uniform global parameter of the program by name.
void gShaderProgram::setUniform(const std::string& name,
                                 float v0, float v1, float v2)
{
    GLint location = getUniformLocation(name);
    if(GLEW_VERSION_2_0) {
        glUniform3f(location, v0, v1, v2);
    }
    else {
        glUniform3f(location, v0, v1, v2);
    }
}

// Set a uniform global parameter of the program by name.
void gShaderProgram::setUniform(const std::string& name,
                                 float v0, float v1, float v2, float v3)
{
    GLint location = getUniformLocation(name);
    if(GLEW_VERSION_2_0) {
        glUniform4f(location, v0, v1, v2, v3);
    }
    else {
        glUniform4f(location, v0, v1, v2, v3);
    }
}

// Set a uniform global int
void  gShaderProgram::setUniformi(const std::string& name, int value)
{
	GLint location = getUniformLocation(name);
    if(GLEW_VERSION_2_0) {
        glUniform1i(location, value);
    }
    else {
        glUniform1i(location, value);
    }
}

// Set Arrays!!
void gShaderProgram::setUniformv(const std::string& name, int count, float *values)
{
	GLint location = getUniformLocation(name);
	if (GLEW_VERSION_2_0) {
		glUniform1fv(location, count, values);
	} else {
		glUniform1fv(location, count, values);
	}
}

void gShaderProgram::setUniformv(const std::string& name, int count, int *values)
{
	GLint location = getUniformLocation(name);
	if (GLEW_VERSION_2_0) {
		glUniform1iv(location, count, values);
	} else {
		glUniform1iv(location, count, values);
	}
}

// Set a uniform global parameter matrix by name

void gShaderProgram::setUniformMatrix3f(const std::string& name, bool rowMajor, float *matrix)
{
	GLint location = getUniformLocation(name);
	if (GLEW_VERSION_2_0) {
		glUniformMatrix3fv(location, 1, rowMajor, matrix);
	}
	else {
		glUniformMatrix3fv(location, 1, rowMajor, matrix);
	}
}

void gShaderProgram::setUniformMatrix4f(const std::string& name, bool rowMajor, float *matrix)
{
	GLint location = getUniformLocation(name);
	if (GLEW_VERSION_2_0) {
		glUniformMatrix4fv(location, 1, rowMajor, matrix);
	}
	else {
		glUniformMatrix4fv(location, 1, rowMajor, matrix);
	}
}

/** Attributes!!!  **/


void gShaderProgram::enableVertexAttributeArray(const std::string& name)
{
	GLint location = getAttributeLocation(name);
    if(GLEW_VERSION_2_0) {
        glEnableVertexAttribArray(location);
    }
    else {
        glEnableVertexAttribArray(location);
    }
}

void gShaderProgram::disableVertexAttributeArray(const std::string& name)
{
	GLint location = getAttributeLocation(name);
    if(GLEW_VERSION_2_0) {
        glDisableVertexAttribArray(location);
    }
    else {
        glDisableVertexAttribArray(location);
    }
}

void gShaderProgram::vertexAttributePointer(const std::string& name, GLint size, GLenum type,
							   GLboolean normalized, GLsizei stride, const void *pointer)
{
	GLint location = getAttributeLocation(name);
    if(GLEW_VERSION_2_0) {
		glVertexAttribPointer(location, size, type, normalized, stride, pointer);
    }
    else {
		glVertexAttribPointer(location, size, type, normalized, stride, pointer);
    }
}

//Get functions
void gShaderProgram::getProgramiv(GLenum pname, GLint *params)
{
	glGetProgramiv(programid, pname, params);
}

#ifndef USE_GLES
void gShaderProgram::setProgramiv(GLenum pname, GLint param)
{
	glProgramParameteriEXT(programid, pname, param);
}
#endif

void gShaderProgram::linkProgram()
{
    glLinkProgram(programid);
}

// Helper routine - get the location for a uniform shader parameter.
GLint gShaderProgram::getUniformLocation(const std::string& name)
{
    if(GLEW_VERSION_2_0) {
        return glGetUniformLocation(programid, name.c_str());
    }
#ifndef __APPLE__
    else {
        return glgetUniformLocation(programid, name.c_str());
    }
#endif
}


GLint gShaderProgram::getAttributeLocation(const std::string& name)
{
	if(GLEW_VERSION_2_0) {
        return glGetAttribLocation(programid, name.c_str());
    }
#ifndef __APPLE__
    else {
        return glGetAttribLocation(programid, name.c_str());
    }
#endif
}