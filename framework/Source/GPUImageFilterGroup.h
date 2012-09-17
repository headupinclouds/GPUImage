/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageFilterGroup_h_
#define _GPUImageFilterGroup_h_

#include "GPUImageOutput.h"
#include "GPUImageFilter.h"
#include <vector>

class GPUImageFilterGroup : public GPUImageOutput, public GPUImageInput
{
public:
    GPUImageFilterGroup();
    ~GPUImageFilterGroup();

    // Filter management
    void addFilter(GPUImageOutput* newFilter);
    GPUImageOutput* filterAtIndex(gpu_uint filterIndex);
    int getFilterCount();

    // newCGImageFromCurrentlyProcessedOutputWithOrientation
    void prepareForImageCapture();

    void setTargetToIgnoreForUpdates(GPUImageInput* target);
    void addTarget(GPUImageInput* newTarget, gpu_int textureLocation);
    void removeTarget(GPUImageInput* targetToRemove);
    void removeAllTargets();

    void newFrameReadyAtTime(gpu_time frameTime, gpu_int textureIndex);
    void setInputTexture(gpu_uint newInputTexture, gpu_int textureIndex);
    gpu_int nextAvailableTextureIndex();
    void setInputSize(gpu_float_size newSize, gpu_int textureIndex);
    void setInputRotation(GPUImageRotationMode newInputRotation, gpu_int textureIndex);
    gpu_float_size maximumOutputSize();
    void endProcessing();

protected:
    std::vector<GPUImageOutput*> filters_;
    GPUImageOutput* terminalFilter_;
    std::vector<GPUImageInput*> initialFilters_;
    GPUImageInput* inputFilterToIgnoreForUpdates_;
};

#endif // _GPUImageFilterGroup_h_