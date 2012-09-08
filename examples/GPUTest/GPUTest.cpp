#include <iostream>
#include "GPUImageOpenGLESContext.h"

int main (int argc, char** argv) {
    std::cout << "GPUImage EGL Context Example" << std::endl;

    GPUImageOpenGLESContext& glesContext = GPUImageOpenGLESContext::sharedImageProcessingOpenGLESContext();
    
    glesContext.getContext();

    return 0;
} 