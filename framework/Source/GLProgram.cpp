/**
 *
 */
 
#include "GLProgram.h"
#include <algorithm>

GLProgram::GLProgram() : 
    initialized_(false), 
    program_(0), 
    vertexShader_(0), 
    fragmentShader_(0) {

}

GLProgram::~GLProgram() {
    destroy();
}

bool GLProgram::initWithVertexShaderString(const std::string& vertexShaderString, const std::string& fragmentShaderString) {
        
    initialized_ = false;

    attributes_.clear();
    uniforms_.clear();
    program_ = glCreateProgram();

    if (!compileShader(vertexShader_, GL_VERTEX_SHADER, vertexShaderString)) {
        //NSLog(@"Failed to compile vertex shader");
        return false;
    }

    // Create and compile fragment shader
    if (!compileShader(fragmentShader_, GL_FRAGMENT_SHADER, fragmentShaderString)) {
        //NSLog(@"Failed to compile fragment shader");
        return false;
    }

    glAttachShader(program_, vertexShader_);
    glAttachShader(program_, fragmentShader_);

    return true;
}

bool GLProgram::initWithVertexShaderFilename(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) {
    return false;
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
            GLchar *log = new GLchar[logLength];
            glGetShaderInfoLog(shader, logLength, &logLength, log);
            //TODO: NSLog(@"Shader compile log:\n%s", log);
            delete [] log;
        }
	}	
	
    return status == GL_TRUE;
}

void GLProgram::addAttribute(const std::string& attributeName) {

    std::vector<std::string>::const_iterator found = std::find(attributes_.begin(), attributes_.end(), attributeName); 
    if (found == attributes_.end()) {

        // the attribute was not found, add it to the end of attributes_ and bind it
        attributes_.push_back(attributeName);

        size_t indexOfAttribute = attributes_.size() - 1;

        glBindAttribLocation(program_, indexOfAttribute, attributeName.c_str());
    }
}

GLuint GLProgram::getAttributeIndex(const std::string& attributeName) {

    // TODO: take care of element not found?
    std::vector<std::string>::const_iterator found = std::find(attributes_.begin(), attributes_.end(), attributeName); 

    return (found - attributes_.begin());
}

GLuint GLProgram::getUniformIndex(const std::string& uniformName) {

    return glGetUniformLocation(program_, uniformName.c_str());
}

bool GLProgram::link() {

    GLint status;

    glLinkProgram(program_);

    glGetProgramiv(program_, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        return false;

    if (vertexShader_) {
        glDeleteShader(vertexShader_);
        vertexShader_ = 0;
    }

    if (fragmentShader_) {
        glDeleteShader(fragmentShader_);
        fragmentShader_ = 0;
    }

    initialized_ = true;
    
    return true;
}

void GLProgram::use() {
    glUseProgram(program_);
}

void GLProgram::validate() {

    GLint logLength;

    glValidateProgram(program_);
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = new GLchar[logLength];
        glGetProgramInfoLog(program_, logLength, &logLength, log);
        //NSLog(@"program_ validate log:\n%s", log);
        delete [] log;
    }	
}

void GLProgram::destroy() {
    
    if (vertexShader_)
        glDeleteShader(vertexShader_);

    if (fragmentShader_)
        glDeleteShader(fragmentShader_);

    if (program_)
        glDeleteProgram(program_);
}

void GLProgram::logForOpenGLObject(std::string& log, GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc) {

    GLint logLength = 0, charsWritten = 0;

    infoFunc(object, GL_INFO_LOG_LENGTH, &logLength);    
    if (logLength < 1)
        return;

    GLchar* logBytes = new GLchar[logLength];
    logFunc(object, logLength, &charsWritten, logBytes);
    log = std::string(logBytes, logLength);
    delete [] logBytes;
}

void GLProgram::getVertexShaderLog(std::string& log) {

    logForOpenGLObject(log, vertexShader_, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
}

void GLProgram::getFragmentShaderLog(std::string& log) {

    logForOpenGLObject(log, fragmentShader_, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
}

void GLProgram::getProgramLog(std::string& log) {

    logForOpenGLObject(log, program_, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
}

bool GLProgram::isInitialized() const {
    return initialized_;
}