/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImagePicture.h"
#include "GPUImageOpenGLESContext.h"
#include "GPUImageInput.h"
#include "ImageContainer.h"
#include "Common.h"
#include <math.h>

GPUImagePicture::GPUImagePicture() 
    : GPUImageOutput(), hasProcessedImage_(false) {

    pixelSizeOfImage_.width = 0.0f;
    pixelSizeOfImage_.height = 0.0f;
}

GPUImagePicture::~GPUImagePicture() {

}

bool GPUImagePicture::initialize(ImageContainer* imageSource, bool smoothlyScaleOutput) {

    if (!textureForOutput()) {
        return false;
    }

    setShouldSmoothlyScaleOutput(smoothlyScaleOutput);

    // TODO: Dispatch this whole thing asynchronously to move image loading off main thread
    float widthOfImage = static_cast<float>(imageSource->getWidth());
    float heightOfImage = static_cast<float>(imageSource->getHeight());
    pixelSizeOfImage_.width = widthOfImage;
    pixelSizeOfImage_.height = heightOfImage;

    gpu_float_size pixelSizeToUseForTexture = pixelSizeOfImage_;

    // should be default false ?
    bool shouldRedrawUsingCoreGraphics = false;

    // For now, deal with images larger than the maximum texture size by resizing to be within that limit
    gpu_float_size scaledImageSizeToFitOnGPU = GPUImageOpenGLESContext::sizeThatFitsWithinATextureForSize(pixelSizeOfImage_);
    if (!FloatSizeEqualToSize(scaledImageSizeToFitOnGPU, pixelSizeOfImage_)) {
        pixelSizeOfImage_ = scaledImageSizeToFitOnGPU;
        pixelSizeToUseForTexture = pixelSizeOfImage_;
        shouldRedrawUsingCoreGraphics = true;
    }

    if (shouldSmoothlyScaleOutput()) {
        // In order to use mipmaps, you need to provide power-of-two textures, so convert to the next largest power of two and stretch to fill
        float powerClosestToWidth = ceil(gpuimage_log2(pixelSizeOfImage_.width));
        float powerClosestToHeight = ceil(gpuimage_log2(pixelSizeOfImage_.height));

        pixelSizeToUseForTexture.width  = pow(2.0f, powerClosestToWidth);
        pixelSizeToUseForTexture.height = pow(2.0f, powerClosestToHeight);

        shouldRedrawUsingCoreGraphics = true;
    }

    GLubyte* imageData = NULL;

    if (shouldRedrawUsingCoreGraphics) {
        // resize the image
        imageSource->resize((int)pixelSizeToUseForTexture.width, (int)pixelSizeToUseForTexture.height);

        imageData = imageSource->getRawBytes();
    }
    else
    {
        // Access the raw image bytes directly
        imageData = imageSource->getRawBytes();
    }    


    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
    [GPUImageOpenGLESContext useImageProcessingContext];

    glBindTexture(GL_TEXTURE_2D, outputTexture);
    if (self.shouldSmoothlyScaleOutput)
    {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)pixelSizeToUseForTexture.width, (int)pixelSizeToUseForTexture.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, imageData);

    if (self.shouldSmoothlyScaleOutput)
    {
    glGenerateMipmap(GL_TEXTURE_2D);
    }        
    });

    if (shouldRedrawUsingCoreGraphics)
    {
    free(imageData);
    }
    else
    {
    CFRelease(dataFromImageDataProvider);
    }*/
    GPUImageOpenGLESContext::useImageProcessingContext();

    glBindTexture(GL_TEXTURE_2D, textureForOutput());
    if (shouldSmoothlyScaleOutput()) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    
    // looks like GL_BGRA is non standard
    glTexImage2D(GL_TEXTURE_2D, 0, imageSource->getFormat(), (int)pixelSizeToUseForTexture.width, 
        (int)pixelSizeToUseForTexture.height, 0, imageSource->getFormat(), GL_UNSIGNED_BYTE, imageData);

    if (shouldSmoothlyScaleOutput()) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    // End runSynchronouslyOnVideoProcessingQueue block

    // imageData belongs to imageSource which will take care of it!
    /*   if (shouldRedrawUsingCoreGraphics) {
        free(imageData);
    }
    else
    {
        CFRelease(dataFromImageDataProvider);
    }*/

    return true;
}

void GPUImagePicture::processImage() {

    hasProcessedImage_ = true;

    // TODO: 
    //dispatch_async([GPUImageOpenGLESContext sharedOpenGLESQueue], ^{
    size_t numberTargets = targets_.size();

    for (size_t k = 0; k < numberTargets; k++) {

        GPUImageInput* currentTarget = targets_[k];
        gpu_int textureIndexOfTarget = targetTextureIndices_[k];
           
        currentTarget->setInputSize(pixelSizeOfImage_, textureIndexOfTarget);

        gpu_time todoTiming;    // TODO: put some correct time in there?
        currentTarget->newFrameReadyAtTime(todoTiming, textureIndexOfTarget);
    }   
}

gpu_float_size GPUImagePicture::getOutputImageSize() {
    return pixelSizeOfImage_;
}

void GPUImagePicture::addTarget(GPUImageInput* newTarget, gpu_int textureLocation) {
    
    GPUImageOutput::addTarget(newTarget, textureLocation);

    if (hasProcessedImage_) {
        newTarget->setInputSize(pixelSizeOfImage_, textureLocation);

        gpu_time todoTiming;    // TODO: put some correct time in there?
        newTarget->newFrameReadyAtTime(todoTiming, textureLocation);
    }
}
