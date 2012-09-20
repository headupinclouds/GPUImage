/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImageAlphaBlendFilter.h"
#include "GLProgram.h"

const std::string GPUImageAlphaBlendFilter::kGPUImageAlphaBlendFragmentShaderString("\
 varying highp vec2 textureCoordinate;\
 varying highp vec2 textureCoordinate2;\
\
 uniform sampler2D inputImageTexture;\
 uniform sampler2D inputImageTexture2;\
 \
 uniform lowp float mixturePercent;\
\
 void main()\
 {\
	 lowp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\
	 lowp vec4 textureColor2 = texture2D(inputImageTexture2, textureCoordinate2);\
	 \
	 gl_FragColor = vec4(mix(textureColor.rgb, textureColor2.rgb, textureColor2.a * mixturePercent), textureColor.a);\
 }"
);

GPUImageAlphaBlendFilter::GPUImageAlphaBlendFilter() :
    GPUImageTwoInputFilter(),
    mix_(0.0),
    mixUniform_(0) 
{

    GPUImageTwoInputFilter::initWithFragmentShaderFromString(kGPUImageAlphaBlendFragmentShaderString);
    
    mixUniform_ = filterProgram_->getUniformIndex("mixturePercent");
    mix_ = 0.5f;    
}

GPUImageAlphaBlendFilter::~GPUImageAlphaBlendFilter() {

}

void GPUImageAlphaBlendFilter::setMix(gpu_float newValue) {
    mix_ = newValue;
    
    setFloat(mix_, mixUniform_, filterProgram_);
}
