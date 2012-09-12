/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageGrayscaleFilter_h_
#define _GPUImageGrayscaleFilter_h_

#include "GPUImageFilter.h"

/** Converts an image to grayscale (a slightly faster implementation of the saturation filter, without the ability to vary the color contribution)
 */
class GPUImageGrayscaleFilter : public GPUImageFilter {
public:
    GPUImageGrayscaleFilter();
    ~GPUImageGrayscaleFilter();

    static const std::string kGPUImageLuminanceFragmentShaderString;
};

#endif // _GPUImageGrayscaleFilter_h_