/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImagePicture_h_
#define _GPUImagePicture_h_

#include "GPUImageOutput.h"

// forward declarations
class ImageContainer;

class GPUImagePicture : public GPUImageOutput {
public:
    GPUImagePicture();
    ~GPUImagePicture();

    bool initialize(ImageContainer* imageSource, bool smoothlyScaleOutput = false);

    void processImage();
    gpu_float_size getOutputImageSize();

    using GPUImageOutput::addTarget;
    void addTarget(GPUImageInput* newTarget, gpu_int textureLocation);

private:
    gpu_float_size pixelSizeOfImage_;
    bool hasProcessedImage_;
};

#endif // _GPUImagePicture_h_
