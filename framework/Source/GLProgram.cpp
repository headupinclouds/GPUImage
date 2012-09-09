/**
 *
 */
 
#include "GLProgram.h"

int GLProgram::initWithVertexShaderString(const std::string& vertexShaderString, const std::string& fragmentShaderString) {
    return 0;
}

int GLProgram::initWithVertexShaderFilename(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) {
    return 0;
}

void GLProgram::addAttribute(const std::string& attributeName) {

}

GLuint GLProgram::getAttributeIndex(const std::string& attributeName) {
    return 0;
}

GLuint GLProgram::getUniformIndex(const std::string& uniformName) {
    return 0;
}

bool GLProgram::link() {
    return false;
}

void GLProgram::use() {

}

void GLProgram::getVertexShaderLog(std::string& log) {

}

void GLProgram::getFragmentShaderLog(std::string& log) {

}

void GLProgram::getProgramLog(std::string& log) {

}

void GLProgram::validate() {

}