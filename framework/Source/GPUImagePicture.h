/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

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

#ifdef portcode
// Initialization and teardown
- (id)initWithImage:(UIImage *)newImageSource;
- (id)initWithCGImage:(CGImageRef)newImageSource;
- (id)initWithImage:(UIImage *)newImageSource smoothlyScaleOutput:(BOOL)smoothlyScaleOutput;
- (id)initWithCGImage:(CGImageRef)newImageSource smoothlyScaleOutput:(BOOL)smoothlyScaleOutput;
#endif
