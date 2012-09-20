/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImageBoxBlurFilter.h"
#include "GLProgram.h"

const std::string GPUImageBoxBlurFilter::kGPUImageBoxBlurVertexShaderString("\
\
 attribute vec4 position;\
 attribute vec2 inputTextureCoordinate;\
\
 uniform float texelWidthOffset;\
 uniform float texelHeightOffset;\
 uniform highp float blurSize;\
\
 varying vec2 centerTextureCoordinate;\
 varying vec2 oneStepLeftTextureCoordinate;\
 varying vec2 twoStepsLeftTextureCoordinate;\
 varying vec2 oneStepRightTextureCoordinate;\
 varying vec2 twoStepsRightTextureCoordinate;\
\
 void main()\
 {\
     gl_Position = position;\
\
     vec2 firstOffset = vec2(1.5 * texelWidthOffset, 1.5 * texelHeightOffset) * blurSize;\
     vec2 secondOffset = vec2(3.5 * texelWidthOffset, 3.5 * texelHeightOffset) * blurSize;\
\
     centerTextureCoordinate = inputTextureCoordinate;\
     oneStepLeftTextureCoordinate = inputTextureCoordinate - firstOffset;\
     twoStepsLeftTextureCoordinate = inputTextureCoordinate - secondOffset;\
     oneStepRightTextureCoordinate = inputTextureCoordinate + firstOffset;\
     twoStepsRightTextureCoordinate = inputTextureCoordinate + secondOffset;\
 }"
);


const std::string GPUImageBoxBlurFilter::kGPUImageBoxBlurFragmentShaderString("\
\
 precision highp float;\
\
 uniform sampler2D inputImageTexture;\
 \
 varying vec2 centerTextureCoordinate;\
 varying vec2 oneStepLeftTextureCoordinate;\
 varying vec2 twoStepsLeftTextureCoordinate;\
 varying vec2 oneStepRightTextureCoordinate;\
 varying vec2 twoStepsRightTextureCoordinate;\
 \
 void main()\
 {\
     lowp vec4 fragmentColor = texture2D(inputImageTexture, centerTextureCoordinate) * 0.2;\
     fragmentColor += texture2D(inputImageTexture, oneStepLeftTextureCoordinate) * 0.2;\
     fragmentColor += texture2D(inputImageTexture, oneStepRightTextureCoordinate) * 0.2;\
     fragmentColor += texture2D(inputImageTexture, twoStepsLeftTextureCoordinate) * 0.2;\
     fragmentColor += texture2D(inputImageTexture, twoStepsRightTextureCoordinate) * 0.2;\
     \
     gl_FragColor = fragmentColor;\
 }"
);

GPUImageBoxBlurFilter::GPUImageBoxBlurFilter() :
    GPUImageTwoPassTextureSamplingFilter(),
    firstBlurSizeUniform_(0),
    secondBlurSizeUniform_(0),
    blurSize_(0.0f)
{
    
    GPUImageTwoPassTextureSamplingFilter::initWithFirstStageVertexShaderFromString(kGPUImageBoxBlurVertexShaderString, kGPUImageBoxBlurFragmentShaderString, 
        kGPUImageBoxBlurVertexShaderString, kGPUImageBoxBlurFragmentShaderString);

    firstBlurSizeUniform_ = filterProgram_->getUniformIndex("blurSize");
    secondBlurSizeUniform_ = secondFilterProgram_->getUniformIndex("blurSize");

    blurSize_ = 1.0f;
}

GPUImageBoxBlurFilter::~GPUImageBoxBlurFilter() {

}

void GPUImageBoxBlurFilter::setBlurSize(gpu_float blurSize) {
    
    blurSize_ = blurSize;
    
    setFloat(blurSize_, firstBlurSizeUniform_, filterProgram_);
    setFloat(blurSize_, secondBlurSizeUniform_, secondFilterProgram_);
}

gpu_float GPUImageBoxBlurFilter::getBlurSize() const {
    return blurSize_;
}

