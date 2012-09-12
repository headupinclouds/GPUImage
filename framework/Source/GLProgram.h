/*  
 *  This is Jeff LaMarche's GLProgram OpenGL shader wrapper class from his OpenGL ES 2.0 book.
 *  A description of this can be found at his page on the topic:
 *  http://iphonedevelopment.blogspot.com/2010/11/opengl-es-20-for-ios-chapter-4.html
 *  I've extended this to be able to take programs as NSStrings in addition to files, for baked-in shaders
 */

#ifndef _GLProgram_h_
#define _GLProgram_h_

#include <GLES2/gl2.h>
#include <string>
#include <vector>

typedef void (*GLInfoFunction)(GLuint program, GLenum pname,  GLint* params);
typedef void (*GLLogFunction) (GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog);

// Use http://stackoverflow.com/questions/2795044/easy-framework-for-opengl-shaders-in-c-c ?
class GLProgram { 
public:
    GLProgram();
    ~GLProgram();

    int initWithVertexShaderString(const std::string& vertexShaderString, const std::string& fragmentShaderString);
    int initWithVertexShaderFilename(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
    bool compileShader(GLuint& shader, GLenum type, const std::string& shaderString);
    void addAttribute(const std::string& attributeName);
    GLuint getAttributeIndex(const std::string& attributeName);
    GLuint getUniformIndex(const std::string& uniformName);
    bool link();
    void use();
    void validate();
    void destroy();

    void logForOpenGLObject(std::string& log, GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc);
    void getVertexShaderLog(std::string& log);
    void getFragmentShaderLog(std::string& log);
    void getProgramLog(std::string& log);

    bool isInitialized() const;

private:
    bool initialized_;
    GLuint program_;
    GLuint vertexShader_;
    GLuint fragmentShader_;
    std::vector<std::string> attributes_;
    std::vector<std::string> uniforms_;
};

#endif // _GLProgram_h_