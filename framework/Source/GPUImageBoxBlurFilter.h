/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageBoxBlurFilter_h_
#define _GPUImageBoxBlurFilter_h_

#include "GPUImageTwoPassTextureSamplingFilter.h"
#include <string>

/** A hardware-accelerated 9-hit box blur of an image
 */
class GPUImageBoxBlurFilter : public GPUImageTwoPassTextureSamplingFilter
{
public:
    GPUImageBoxBlurFilter();
    ~GPUImageBoxBlurFilter();

    void setBlurSize(gpu_float blurSize);
    gpu_float getBlurSize() const;

    static const std::string kGPUImageBoxBlurVertexShaderString;
    static const std::string kGPUImageBoxBlurFragmentShaderString;

private:
    GLint firstBlurSizeUniform_;
    GLint secondBlurSizeUniform_;

    // A scaling for the size of the applied blur, default of 1.0
    gpu_float blurSize_;
};

#endif // GPUImageBoxBlurFilter