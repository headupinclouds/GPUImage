#include "GPUImageOpenGLESContext.h"

GPUImageOpenGLESContext& GPUImageOpenGLESContext::sharedImageProcessingOpenGLESContext() {
    static GPUImageOpenGLESContext instance; // Guaranteed to be destroyed.

    // Instantiated on first use.
    return instance;
}

EGLContext GPUImageOpenGLESContext::getContext() {

    if (context == EGL_NO_CONTEXT) {

    }

    return context;
}

GPUImageOpenGLESContext::GPUImageOpenGLESContext() 
    : context(EGL_NO_CONTEXT) {

}