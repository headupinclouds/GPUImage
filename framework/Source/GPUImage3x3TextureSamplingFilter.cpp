/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImage3x3TextureSamplingFilter.h"
#include "GLProgram.h"
#include "GPUImageOpenGLESContext.h"

const std::string GPUImage3x3TextureSamplingFilter::kGPUImageNearbyTexelSamplingVertexShaderString("\
\
 attribute vec4 position; \
 attribute vec4 inputTextureCoordinate; \
 \
 uniform highp float texelWidth; \
 uniform highp float texelHeight; \
 varying vec2 textureCoordinate; \
 varying vec2 leftTextureCoordinate; \
 varying vec2 rightTextureCoordinate;  \
 \
 varying vec2 topTextureCoordinate; \
 varying vec2 topLeftTextureCoordinate; \
 varying vec2 topRightTextureCoordinate; \
 \
 varying vec2 bottomTextureCoordinate; \
 varying vec2 bottomLeftTextureCoordinate; \
 varying vec2 bottomRightTextureCoordinate; \
 \
 void main() \
 { \
     gl_Position = position; \
     \
     vec2 widthStep = vec2(texelWidth, 0.0); \
     vec2 heightStep = vec2(0.0, texelHeight); \
     vec2 widthHeightStep = vec2(texelWidth, texelHeight); \
     vec2 widthNegativeHeightStep = vec2(texelWidth, -texelHeight); \
     \
     textureCoordinate = inputTextureCoordinate.xy; \
     leftTextureCoordinate = inputTextureCoordinate.xy - widthStep; \
     rightTextureCoordinate = inputTextureCoordinate.xy + widthStep; \
     \
     topTextureCoordinate = inputTextureCoordinate.xy - heightStep; \
     topLeftTextureCoordinate = inputTextureCoordinate.xy - widthHeightStep; \
     topRightTextureCoordinate = inputTextureCoordinate.xy + widthNegativeHeightStep; \
     \
     bottomTextureCoordinate = inputTextureCoordinate.xy + heightStep; \
     bottomLeftTextureCoordinate = inputTextureCoordinate.xy - widthNegativeHeightStep; \
     bottomRightTextureCoordinate = inputTextureCoordinate.xy + widthHeightStep; \
 }"
);

GPUImage3x3TextureSamplingFilter::GPUImage3x3TextureSamplingFilter() 
    : GPUImageFilter(),texelWidthUniform_(0), texelHeightUniform_(0), texelWidth_(0), texelHeight_(0),
    hasOverriddenImageSizeFactor_(false) {

}

GPUImage3x3TextureSamplingFilter::~GPUImage3x3TextureSamplingFilter() {

}


void GPUImage3x3TextureSamplingFilter::initWithFragmentShaderFromString(const std::string& fragmentShaderString) {

    initWithVertexShaderFromString(kGPUImageNearbyTexelSamplingVertexShaderString, fragmentShaderString);
    
    texelWidthUniform_ = filterProgram_->getUniformIndex("texelWidth");
    texelHeightUniform_ = filterProgram_->getUniformIndex("texelHeight");
}

void GPUImage3x3TextureSamplingFilter::setupFilterForSize(gpu_float_size filterFrameSize) {

    if (!hasOverriddenImageSizeFactor_) {
        texelWidth_ = 1.0 / filterFrameSize.width;
        texelHeight_ = 1.0 / filterFrameSize.height;
        
        // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
        // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
        /*runSynchronouslyOnVideoProcessingQueue(^{
            [GPUImageOpenGLESContext setActiveShaderProgram:filterProgram];
            if (GPUImageRotationSwapsWidthAndHeight(inputRotation))
            {
                glUniform1f(texelWidthUniform, _texelHeight);
                glUniform1f(texelHeightUniform, _texelWidth);
            }
            else
            {
                glUniform1f(texelWidthUniform, _texelWidth);
                glUniform1f(texelHeightUniform, _texelHeight);
            }
        });*/
        GPUImageOpenGLESContext::setActiveShaderProgram(filterProgram_);
        if (GPUImageRotationSwapsWidthAndHeight(inputRotation_)) {
            glUniform1f(texelWidthUniform_, texelHeight_);
            glUniform1f(texelHeightUniform_, texelWidth_);
        }
        else
        {
            glUniform1f(texelWidthUniform_, texelWidth_);
            glUniform1f(texelHeightUniform_, texelHeight_);
        }
        // End runSynchronouslyOnVideoProcessingQueue block

    }
}

void GPUImage3x3TextureSamplingFilter::setTexelWidth(gpu_float texelWidth) {
    
    hasOverriddenImageSizeFactor_ = true;
    texelWidth_ = texelWidth;
    
    setFloat(texelWidth_, texelWidthUniform_, filterProgram_);
}

void GPUImage3x3TextureSamplingFilter::setTexelHeight(gpu_float texelHeight) {

    hasOverriddenImageSizeFactor_ = true;
    texelHeight_ = texelHeight;
    
    setFloat(texelHeight_, texelHeightUniform_, filterProgram_);
}