/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageAverageColor_h_
#define _GPUImageAverageColor_h_

#include "GPUImageFilter.h"
#include <string>
#include <vector>

class GPUImageAverageColor : public GPUImageFilter
{
public:
    GPUImageAverageColor();
    ~GPUImageAverageColor();

    void initializeOutputTexture();
    void deleteOutputTexture();
    void recreateFilterFBO();
    void createFilterFBOofSize(gpu_float_size currentFBOSize);
    void destroyFilterFBO();
    void renderToTextureWithVertices(const GLfloat* vertices, const GLfloat* textureCoordinates, GLuint sourceTexture);

    void prepareForImageCapture();
    void setInputRotation(GPUImageRotationMode newInputRotation, gpu_int textureIndex);
    void extractAverageColorAtFrameTime(gpu_time frameTime);
/*
// This block is called on the completion of color averaging for a frame
@property(nonatomic, copy) void(^colorAverageProcessingFinishedBlock)(CGFloat redComponent, CGFloat greenComponent, CGFloat blueComponent, CGFloat alphaComponent, CMTime frameTime);

- (void)extractAverageColorAtFrameTime:(CMTime)frameTime;
*/
    static const std::string kGPUImageColorAveragingVertexShaderString;
    static const std::string kGPUImageColorAveragingFragmentShaderString;

private:
    GLint texelWidthUniform_; 
    GLint texelHeightUniform_;
    
    gpu_uint numberOfStages_;
    std::vector<gpu_uint> stageTextures_;
    std::vector<gpu_uint> stageFramebuffers_; 
    std::vector<gpu_float_size> stageSizes_;

    GLubyte* rawImagePixels_;
};

#endif // _GPUImageAverageColor_h_
