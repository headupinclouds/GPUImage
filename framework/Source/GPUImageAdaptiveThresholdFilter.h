/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageAdaptiveThresholdFilter_h_
#define _GPUImageAdaptiveThresholdFilter_h_

#include "GPUImageFilterGroup.h"
#include <string>

// forward declarations
class GPUImageBoxBlurFilter;

class GPUImageAdaptiveThresholdFilter : public GPUImageFilterGroup
{
public:
    GPUImageAdaptiveThresholdFilter();
    ~GPUImageAdaptiveThresholdFilter();

    const static std::string kGPUImageAdaptiveThresholdFragmentShaderString;

    void setBlurSize(gpu_float blurSize);
    gpu_float getBlurSize();

private:
    /** A multiplier for the background averaging blur size, ranging from 0.0 on up, with a default of 1.0
     */
    gpu_float blurSize_;
    GPUImageBoxBlurFilter* boxBlurFilter_;
};

#endif