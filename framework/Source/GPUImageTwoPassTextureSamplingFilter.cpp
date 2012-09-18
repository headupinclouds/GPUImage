/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImageTwoPassTextureSamplingFilter.h"
#include "GLProgram.h"

GPUImageTwoPassTextureSamplingFilter::GPUImageTwoPassTextureSamplingFilter() {

}

GPUImageTwoPassTextureSamplingFilter::~GPUImageTwoPassTextureSamplingFilter() {

}

void GPUImageTwoPassTextureSamplingFilter::initWithFirstStageVertexShaderFromString(const std::string& firstStageVertexShaderString, const std::string& firstStageFragmentShaderString, 
                                                                                    const std::string& secondStageVertexShaderString, const std::string& secondStageFragmentShaderString) {

    GPUImageTwoPassFilter::initWithFirstStageVertexShaderFromString(firstStageVertexShaderString, firstStageFragmentShaderString, secondStageVertexShaderString, secondStageFragmentShaderString);
    
    verticalPassTexelWidthOffsetUniform_ = filterProgram_->getUniformIndex("texelWidthOffset");
    verticalPassTexelHeightOffsetUniform_ = filterProgram_->getUniformIndex("texelHeightOffset");
    
    horizontalPassTexelWidthOffsetUniform_ = secondFilterProgram_->getUniformIndex("texelWidthOffset");
    horizontalPassTexelHeightOffsetUniform_ = secondFilterProgram_->getUniformIndex("texelHeightOffset");
}

void GPUImageTwoPassTextureSamplingFilter::setUniformsForProgramAtIndex(gpu_uint programIndex) {

    if (programIndex == 0) {
        glUniform1f(verticalPassTexelWidthOffsetUniform_, verticalPassTexelWidthOffset_);
        glUniform1f(verticalPassTexelHeightOffsetUniform_, verticalPassTexelHeightOffset_);
    }
    else
    {
        glUniform1f(horizontalPassTexelWidthOffsetUniform_, horizontalPassTexelWidthOffset_);
        glUniform1f(horizontalPassTexelHeightOffsetUniform_, horizontalPassTexelHeightOffset_);
    }
}

void GPUImageTwoPassTextureSamplingFilter::setupFilterForSize(gpu_float_size filterFrameSize) {

    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
        // The first pass through the framebuffer may rotate the inbound image, so need to account for that by changing up the kernel ordering for that pass
        if (GPUImageRotationSwapsWidthAndHeight(inputRotation))
        {
            verticalPassTexelWidthOffset = 1.0 / filterFrameSize.height;
            verticalPassTexelHeightOffset = 0.0;
        }
        else
        {
            verticalPassTexelWidthOffset = 0.0;
            verticalPassTexelHeightOffset = 1.0 / filterFrameSize.height;
        }
        
        horizontalPassTexelWidthOffset = 1.0 / filterFrameSize.width;
        horizontalPassTexelHeightOffset = 0.0;
    });*/
    // The first pass through the framebuffer may rotate the inbound image, so need to account for that by changing up the kernel ordering for that pass
    if (GPUImageRotationSwapsWidthAndHeight(inputRotation_)) {
        verticalPassTexelWidthOffset_ = 1.0f / filterFrameSize.height;
        verticalPassTexelHeightOffset_ = 0.0f;
    }
    else
    {
        verticalPassTexelWidthOffset_ = 0.0f;
        verticalPassTexelHeightOffset_ = 1.0f / filterFrameSize.height;
    }

    horizontalPassTexelWidthOffset_ = 1.0f / filterFrameSize.width;
    horizontalPassTexelHeightOffset_ = 0.0f;
    // End runSynchronouslyOnVideoProcessingQueue block
}
