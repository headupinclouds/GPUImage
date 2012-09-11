#include <iostream>
#include "GPUImageOpenGLESContext.h"
#include "GPUImagePicture.h"
#include "PNGImageContainer.h"

int main (int argc, char** argv) {
    std::cout << "GPUImage EGL Context Example" << std::endl;

    GPUImageOpenGLESContext& glesContext = GPUImageOpenGLESContext::sharedImageProcessingOpenGLESContext();
    
    glesContext.getContext();
    std::cout << "maximumTextureSizeForThisDevice():  " << glesContext.maximumTextureSizeForThisDevice() << std::endl;
    std::cout << "maximumTextureUnitsForThisDevice(): " << glesContext.maximumTextureUnitsForThisDevice() << std::endl;
    glesContext.release();

    PNGImageContainer* imageSource = new PNGImageContainer("mask.png");

    GPUImagePicture* imageToProcess = new GPUImagePicture();
    if (imageToProcess)
        imageToProcess->initialize(imageSource, true);

    return 0;
} 