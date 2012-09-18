/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageTwoPassTextureSamplingFilter_h_
#define _GPUImageTwoPassTextureSamplingFilter_h_

#include "GPUImageTwoPassFilter.h"

class GPUImageTwoPassTextureSamplingFilter : public GPUImageTwoPassFilter
{
public:
    GPUImageTwoPassTextureSamplingFilter();
    ~GPUImageTwoPassTextureSamplingFilter();

    void initWithFirstStageVertexShaderFromString(const std::string& firstStageVertexShaderString, const std::string& firstStageFragmentShaderString, 
        const std::string& secondStageVertexShaderString, const std::string& secondStageFragmentShaderString);

    void setUniformsForProgramAtIndex(gpu_uint programIndex);
    void setupFilterForSize(gpu_float_size filterFrameSize);

private:
    GLint verticalPassTexelWidthOffsetUniform_; 
    GLint verticalPassTexelHeightOffsetUniform_; 
    GLint horizontalPassTexelWidthOffsetUniform_;
    GLint horizontalPassTexelHeightOffsetUniform_;

    GLfloat verticalPassTexelWidthOffset_;
    GLfloat verticalPassTexelHeightOffset_;
    GLfloat horizontalPassTexelWidthOffset_;
    GLfloat horizontalPassTexelHeightOffset_;
};

#endif // _GPUImageTwoPassTextureSamplingFilter_h_
