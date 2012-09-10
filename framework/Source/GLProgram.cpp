/**
 *
 */
 
#include "GLProgram.h"
#include <algorithm>

GLProgram::GLProgram() 
    : initialized(false), program(NULL), vertexShader(NULL), fragmentShader(NULL) {

}

GLProgram::~GLProgram() {
    destroy();
}

int GLProgram::initWithVertexShaderString(const std::string& vertexShaderString, const std::string& fragmentShaderString) {
        
    initialized = false;

    attributes.clear();
    uniforms.clear();
    program = glCreateProgram();

    if (!compileShader(vertexShader, GL_VERTEX_SHADER, vertexShaderString)) {
        //NSLog(@"Failed to compile vertex shader");
    }

    // Create and compile fragment shader
    if (!compileShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderString)) {
        //NSLog(@"Failed to compile fragment shader");
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    return 0;
}

int GLProgram::initWithVertexShaderFilename(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) {
    return 0;
}

bool GLProgram::compileShader(GLuint& shader, GLenum type, const std::string& shaderString) {

    GLint status;
    
    const GLchar* source = shaderString.c_str();
    if (!source) {
        //NSLog(@"Failed to load vertex shader");
        return false;
    }
    
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		
        if (logLength > 0) {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetShaderInfoLog(shader, logLength, &logLength, log);
            //NSLog(@"Shader compile log:\n%s", log);
            free(log);
        }
	}	
	
    return status == GL_TRUE;
}

void GLProgram::addAttribute(const std::string& attributeName) {

    std::vector<std::string>::const_iterator found = std::find(attributes.begin(), attributes.end(), attributeName); 
    if (found == attributes.end()) {

        // the attribute was not found, add it to the end of attributes and bind it
        attributes.push_back(attributeName);

        size_t indexOfAttribute = attributes.size() - 1;

        glBindAttribLocation(program, indexOfAttribute, attributeName.c_str());
    }
}

GLuint GLProgram::getAttributeIndex(const std::string& attributeName) {

    // TODO: take care of element not found?
    std::vector<std::string>::const_iterator found = std::find(attributes.begin(), attributes.end(), attributeName); 

    return (found - attributes.begin());
}

GLuint GLProgram::getUniformIndex(const std::string& uniformName) {

    return glGetUniformLocation(program, uniformName.c_str());
}

bool GLProgram::link() {

    GLint status;

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        return false;

    if (vertexShader) {
        glDeleteShader(vertexShader);
        vertexShader = 0;
    }

    if (fragmentShader) {
        glDeleteShader(fragmentShader);
        fragmentShader = 0;
    }

    initialized = true;
    
    return true;
}

void GLProgram::use() {
    glUseProgram(program);
}

void GLProgram::validate() {

    GLint logLength;

    glValidateProgram(program);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(program, logLength, &logLength, log);
        //NSLog(@"Program validate log:\n%s", log);
        free(log);
    }	
}

void GLProgram::destroy() {
    
    if (vertexShader)
        glDeleteShader(vertexShader);

    if (fragmentShader)
        glDeleteShader(fragmentShader);

    if (program)
        glDeleteProgram(program);
}

void GLProgram::logForOpenGLObject(std::string& log, GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc) {

    GLint logLength = 0, charsWritten = 0;

    infoFunc(object, GL_INFO_LOG_LENGTH, &logLength);    
    if (logLength < 1)
        return;

    char *logBytes = (char*)malloc(logLength);
    logFunc(object, logLength, &charsWritten, logBytes);
    log = std::string(logBytes, logLength);
    free(logBytes);
}

void GLProgram::getVertexShaderLog(std::string& log) {

    logForOpenGLObject(log, vertexShader, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
}

void GLProgram::getFragmentShaderLog(std::string& log) {

    logForOpenGLObject(log, fragmentShader, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
}

void GLProgram::getProgramLog(std::string& log) {

    logForOpenGLObject(log, program, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
}