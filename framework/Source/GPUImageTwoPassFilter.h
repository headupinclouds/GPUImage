/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageTwoPassFilter_h_
#define _GPUImageTwoPassFilter_h_

#include "GPUImageFilter.h"
#include <string>

// forward declarations
class GLProgram;

class GPUImageTwoPassFilter : public GPUImageFilter 
{
public:
    GPUImageTwoPassFilter();
    ~GPUImageTwoPassFilter();

    void initWithFirstStageVertexShaderFromString(const std::string& firstStageVertexShaderString, const std::string& firstStageFragmentShaderString, 
        const std::string& secondStageVertexShaderString, const std::string& secondStageFragmentShaderString);
    void initWithFirstStageFragmentShaderFromString(const std::string& firstStageFragmentShaderString, const std::string& secondStageFragmentShaderString);
    void initializeSecondaryAttributes();

    // from GPUImageOutput
    GLuint textureForOutput();
    void initializeOutputTexture();
    void deleteOutputTexture();
    void createFilterFBOofSize(gpu_float_size currentFBOSize);

    void destroyFilterFBO();
    void setOutputFBO();

    void renderToTextureWithVertices(const GLfloat* vertices, const GLfloat* textureCoordinates, GLuint sourceTexture);
    void prepareForImageCapture();

    // other methods
    void setSecondFilterFBO();

private:
    GLuint secondFilterOutputTexture_;

    GLProgram* secondFilterProgram_;
    GLint secondFilterPositionAttribute_; 
    GLint secondFilterTextureCoordinateAttribute_;

    GLint secondFilterInputTextureUniform_; 
    GLint secondFilterInputTextureUniform2_;
    
    GLuint secondFilterFramebuffer_;
};

#endif // _GPUImageTwoPassFilter_h_