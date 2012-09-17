/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImage3x3TextureSamplingFilter_h_
#define _GPUImage3x3TextureSamplingFilter_h_

#include "GPUImageFilter.h"

class GPUImage3x3TextureSamplingFilter : public GPUImageFilter
{
public:
    GPUImage3x3TextureSamplingFilter();
    virtual ~GPUImage3x3TextureSamplingFilter();

    void initWithFragmentShaderFromString(const std::string& fragmentShaderString);
    void setupFilterForSize(gpu_float_size filterFrameSize);

    static const std::string kGPUImageNearbyTexelSamplingVertexShaderString;

    void setTexelWidth(gpu_float texelWidth);
    void setTexelHeight(gpu_float texelHeight);

private:
    gpu_int texelWidthUniform_;
    gpu_int texelHeightUniform_;
    
    // The texel width and height determines how far out to sample from this texel. 
    // By default, this is the normalized width of a pixel, but this can be overridden 
    // for different effects.
    gpu_float texelWidth_; 
    gpu_float texelHeight_;
    bool hasOverriddenImageSizeFactor_;
};

#endif // _GPUImage3x3TextureSamplingFilter_h_