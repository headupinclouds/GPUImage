/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageAddBlendFilter_h_
#define _GPUImageAddBlendFilter_h_

#include "GPUImageTwoInputFilter.h"
#include <string>

class GPUImageAddBlendFilter : public GPUImageTwoInputFilter 
{
public:
    GPUImageAddBlendFilter();
    ~GPUImageAddBlendFilter();

    static const std::string kGPUImageAddBlendFragmentShaderString;
};

#endif // _GPUImageAddBlendFilter_h_