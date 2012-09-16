/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImage3x3ConvolutionFilter_h_
#define _GPUImage3x3ConvolutionFilter_h_

#include "GPUImage3x3TextureSamplingFilter.h"

/** Runs a 3x3 convolution kernel against the image
 */
class GPUImage3x3ConvolutionFilter : public GPUImage3x3TextureSamplingFilter
{
public:
    GPUImage3x3ConvolutionFilter();
    ~GPUImage3x3ConvolutionFilter();

    void setConvolutionKernel(const GPUMatrix3x3& kernel);

    const static std::string kGPUImage3x3ConvolutionFragmentShaderString;

private:
    /** Convolution kernel to run against the image
     
     The convolution kernel is a 3x3 matrix of values to apply to the pixel and its 8 surrounding pixels.
     The matrix is specified in row-major order, with the top left pixel being one.one and the bottom right three.three
     If the values in the matrix don't add up to 1.0, the image could be brightened or darkened.
     */
    GPUMatrix3x3 convolutionKernel_;
    gpu_int convolutionMatrixUniform_;
};

#endif // _GPUImage3x3ConvolutionFilter_h_



