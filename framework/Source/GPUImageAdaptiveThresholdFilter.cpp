/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImageAdaptiveThresholdFilter.h"
#include "GPUImageGrayscaleFilter.h"
#include "GPUImageTwoInputFilter.h"
#include "GPUImageBoxBlurFilter.h"

const std::string GPUImageAdaptiveThresholdFilter::kGPUImageAdaptiveThresholdFragmentShaderString("\
\
 varying highp vec2 textureCoordinate;\
 varying highp vec2 textureCoordinate2;\
 \
 uniform sampler2D inputImageTexture;\
 uniform sampler2D inputImageTexture2; \
 \
 void main()\
 {\
     highp float blurredInput = texture2D(inputImageTexture, textureCoordinate).r;\
     highp float localLuminance = texture2D(inputImageTexture2, textureCoordinate2).r;\
     highp float thresholdResult = step(blurredInput - 0.05, localLuminance);\
     \
     gl_FragColor = vec4(vec3(thresholdResult), 1.0);\
//     gl_FragColor = vec4(localLuminance, textureColor.r, 0.0, textureColor.w);\
//     gl_FragColor = vec4(localLuminance, localLuminance, localLuminance, 1.0);\
 }"
);

GPUImageAdaptiveThresholdFilter::GPUImageAdaptiveThresholdFilter() 
    : GPUImageFilterGroup(), blurSize_(0), boxBlurFilter_(NULL) {

    // First pass: reduce to luminance
    GPUImageGrayscaleFilter* luminanceFilter = new GPUImageGrayscaleFilter();
    addFilter(luminanceFilter);
    
    // Second pass: perform a box blur
    boxBlurFilter_ = new GPUImageBoxBlurFilter();
    addFilter(boxBlurFilter_);
    
    // Third pass: compare the blurred background luminance to the local value
    GPUImageFilter* adaptiveThresholdFilter = new GPUImageTwoInputFilter();
    adaptiveThresholdFilter->initWithFragmentShaderFromString(kGPUImageAdaptiveThresholdFragmentShaderString);
    addFilter(adaptiveThresholdFilter);
    
    luminanceFilter->addTarget(boxBlurFilter_);
    
    boxBlurFilter_->addTarget(adaptiveThresholdFilter);
    // To prevent double updating of this filter, disable updates from the sharp luminance image side
    luminanceFilter->addTarget(adaptiveThresholdFilter);
    
    initialFilters_.push_back(luminanceFilter);
    terminalFilter_ = adaptiveThresholdFilter;
    
}

void GPUImageAdaptiveThresholdFilter::setBlurSize(gpu_float newValue) {
    boxBlurFilter_->setBlurSize(newValue);
}

gpu_float GPUImageAdaptiveThresholdFilter::getBlurSize() {
    return boxBlurFilter_->getBlurSize();
}
