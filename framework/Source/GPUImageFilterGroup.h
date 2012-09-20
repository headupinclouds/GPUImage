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
    void addFilter(GPUImageFilter* newFilter);
    GPUImageFilter* filterAtIndex(gpu_uint filterIndex);
    int getFilterCount();

    // getCurrentOutput replaces original newCGImageFromCurrentlyProcessedOutputWithOrientation
    GLubyte* getCurrentOutputAsBuffer();
    void prepareForImageCapture();

    void setTargetToIgnoreForUpdates(GPUImageInput* target);
    void addTarget(GPUImageInput* newTarget, gpu_int textureLocation);
    void removeTarget(GPUImageInput* targetToRemove);
    void removeAllTargets();

    gpu_float_size sizeOfFBO();

    void newFrameReadyAtTime(gpu_time frameTime, gpu_int textureIndex);
    void setInputTexture(gpu_uint newInputTexture, gpu_int textureIndex);
    gpu_int nextAvailableTextureIndex();
    void setInputSize(gpu_float_size newSize, gpu_int textureIndex);
    void setInputRotation(GPUImageRotationMode newInputRotation, gpu_int textureIndex);
    gpu_float_size maximumOutputSize();
    void endProcessing();

    // TODO: check where to put shouldIgnoreUpdatesToThisTarget/setShouldIgnoreUpdatesToThisTarget
    // since they need to be here in order for derived classes to be implemented.
    bool shouldIgnoreUpdatesToThisTarget();
    void setShouldIgnoreUpdatesToThisTarget(bool ignore);
    bool enabled();
    void setEnabled(bool enable);

protected:
    bool shouldIgnoreUpdatesToThisTarget_;
    bool enabled_;
    std::vector<GPUImageFilter*> filters_;
    GPUImageFilter* terminalFilter_;
    std::vector<GPUImageFilter*> initialFilters_;
    GPUImageFilter* inputFilterToIgnoreForUpdates_;
};

#endif // _GPUImageFilterGroup_h_