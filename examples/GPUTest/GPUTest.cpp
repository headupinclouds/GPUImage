#include <iostream>
#include "GPUImageOpenGLESContext.h"
#include "GPUImagePicture.h"
#include "PNGImageContainer.h"
#include "GPUImageGrayScaleFilter.h"
#include "GPUImageAmatorkaFilter.h"

int main (int argc, char** argv) {
    std::cout << "GPUImage EGL Context Example" << std::endl;

    GPUImageOpenGLESContext& glesContext = GPUImageOpenGLESContext::sharedImageProcessingOpenGLESContext();
    
    glesContext.getContext();
    std::cout << "maximumTextureSizeForThisDevice():  " << glesContext.maximumTextureSizeForThisDevice() << std::endl;
    std::cout << "maximumTextureUnitsForThisDevice(): " << glesContext.maximumTextureUnitsForThisDevice() << std::endl;

    PNGImageContainer* imageSource = new PNGImageContainer();
    if (!imageSource)
        return -1;

    if (!imageSource->load("test.png")) {
        delete imageSource;

        return -1;
    }

    GPUImagePicture* imageToProcess = new GPUImagePicture();
    if (!imageToProcess)
        return -1;

    if (!imageToProcess->initialize(imageSource, true)) {
        delete imageToProcess;
        delete imageSource;

        return -1;
    }

    /*GPUImageGrayscaleFilter* grayscaleFilter = new GPUImageGrayscaleFilter();
    if (!grayscaleFilter) {
        delete imageToProcess;
        delete imageSource;

        return -1;
    }

    imageToProcess->addTarget(grayscaleFilter);
    imageToProcess->processImage();*/

    GPUImageAmatorkaFilter* amatorkaFilter = new GPUImageAmatorkaFilter();
    if (!amatorkaFilter) {
        delete imageToProcess;
        delete imageSource;

        return -1;
    }

    imageToProcess->addTarget(amatorkaFilter);
    imageToProcess->processImage();

    gpu_float_size processedImageSize = amatorkaFilter->sizeOfFBO();

    GLubyte* buffer = amatorkaFilter->getCurrentOutputAsBuffer();
    PNGImageContainer::writePNG("amatorkaTest.png", buffer, static_cast<gpu_int>(processedImageSize.width), static_cast<gpu_int>(processedImageSize.height), 4);
    delete [] buffer;

    glesContext.release();

    return 0;
} 