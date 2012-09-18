/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageAlphaBlendFilter_h_
#define _GPUImageAlphaBlendFilter_h_

#include "GPUImageTwoInputFilter.h"
#include <string>

class GPUImageAlphaBlendFilter : public GPUImageTwoInputFilter
{
public:
    GPUImageAlphaBlendFilter();
    ~GPUImageAlphaBlendFilter();

    void setMix(gpu_float newValue);

    static const std::string kGPUImageAlphaBlendFragmentShaderString;

private:
    // Mix ranges from 0.0 (only image 1) to 1.0 (only image 2), with 1.0 as the normal level
    gpu_float mix_;
    GLint mixUniform_;
};

#endif // GPUImageAlphaBlendFilter