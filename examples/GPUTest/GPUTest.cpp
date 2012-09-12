#include <iostream>
#include "GPUImageOpenGLESContext.h"
#include "GPUImagePicture.h"
#include "PNGImageContainer.h"
#include "GPUImageGrayScaleFilter.h"

int main (int argc, char** argv) {
    std::cout << "GPUImage EGL Context Example" << std::endl;

    GPUImageOpenGLESContext& glesContext = GPUImageOpenGLESContext::sharedImageProcessingOpenGLESContext();
    
    glesContext.getContext();
    std::cout << "maximumTextureSizeForThisDevice():  " << glesContext.maximumTextureSizeForThisDevice() << std::endl;
    std::cout << "maximumTextureUnitsForThisDevice(): " << glesContext.maximumTextureUnitsForThisDevice() << std::endl;
    glesContext.release();

    PNGImageContainer* imageSource = new PNGImageContainer("mask.png");
    if (!imageSource)
        return -1;

    GPUImagePicture* imageToProcess = new GPUImagePicture();
    if (!imageToProcess)
        return -1;

    if (!imageToProcess->initialize(imageSource, true)) {
        delete imageToProcess;
        delete imageSource;

        return -1;
    }

    GPUImageGrayscaleFilter* grayscaleFilter = new GPUImageGrayscaleFilter();
    if (!grayscaleFilter) {
        delete imageToProcess;
        delete imageSource;

        return -1;
    }

    imageToProcess->addTarget(grayscaleFilter);
    imageToProcess->processImage();

    return 0;
} 