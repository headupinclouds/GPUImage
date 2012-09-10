/**
 * TODO: allow to process images without using a surface
 * TODO: attach a window here?
 * TODO: link against debug EGL libraries ( http://stackoverflow.com/questions/2209929/linking-different-libraries-for-debug-and-release-builds-in-cmake-on-windows )
 */
#include "GPUImageOpenGLESContext.h"
#include "GLProgram.h"

GPUImageOpenGLESContext& GPUImageOpenGLESContext::sharedImageProcessingOpenGLESContext() {

    static GPUImageOpenGLESContext instance; // Guaranteed to be destroyed.

    // Instantiated on first use.
    return instance;
}

void GPUImageOpenGLESContext::setActiveShaderProgram(GLProgram* shaderProgram) {

    // get an instance of the OpenGLES context
    GPUImageOpenGLESContext& glesContext = GPUImageOpenGLESContext::sharedImageProcessingOpenGLESContext();
    //EGLContext context = glesContext.getContext();

    // update the active shader program
    if (glesContext.getCurrentShaderProgram() != shaderProgram) {
        glesContext.setCurrentShaderProgram(shaderProgram);
        shaderProgram->use();
    }
}

GLint GPUImageOpenGLESContext::maximumTextureSizeForThisDevice() {

    GLint maxTextureSize; 
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    return maxTextureSize;
}

GLint GPUImageOpenGLESContext::maximumTextureUnitsForThisDevice() {
        
    GLint maxTextureUnits; 
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    
    return maxTextureUnits;
}

gpu_float_size GPUImageOpenGLESContext::sizeThatFitsWithinATextureForSize(const gpu_float_size& inputSize) {

    GLint maxTextureSize = maximumTextureSizeForThisDevice(); 
    if ( (inputSize.width < maxTextureSize) && (inputSize.height < maxTextureSize) )
    {
        return inputSize;
    }
    
    gpu_float_size adjustedSize;
    if (inputSize.width > inputSize.height)
    {
        adjustedSize.width = (float)maxTextureSize;
        adjustedSize.height = ((float)maxTextureSize / inputSize.width) * inputSize.height;
    }
    else
    {
        adjustedSize.height = (float)maxTextureSize;
        adjustedSize.width = ((float)maxTextureSize / inputSize.height) * inputSize.width;
    }

    return adjustedSize;
}

GLProgram* GPUImageOpenGLESContext::programForVertexShaderString(const std::string& vertexShaderString, const std::string& fragmentShaderString) {
    // TODO: caching mechanism
    //NSString *lookupKeyForShaderProgram = [NSString stringWithFormat:@"V: %@ - F: %@", vertexShaderString, fragmentShaderString];
    GLProgram *programFromCache = NULL; //TODO: = [shaderProgramCache objectForKey:lookupKeyForShaderProgram];

    // no cache hit, initialize a new shader program
    if (programFromCache == NULL) {
        programFromCache = new GLProgram();
        programFromCache->initWithVertexShaderString(vertexShaderString, fragmentShaderString);
        //TODO: [shaderProgramCache setObject:programFromCache forKey:lookupKeyForShaderProgram];
    }
    
    return programFromCache;
}

EGLContext GPUImageOpenGLESContext::getContext() {

    if (context_ == EGL_NO_CONTEXT) {
        // http://www.khronos.org/message_boards/viewtopic.php?t=1001
        // http://source-android.frandroid.com/frameworks/base/opengl/tests/tritex/tritex.cpp
        // http://www.khronos.org/message_boards/viewtopic.php?f=13&t=4612&hilit=EGL_PBUFFER_BIT
        initializeContext(EGL_DEFAULT_DISPLAY);
    }

    return context_;
}

GLProgram* GPUImageOpenGLESContext::getCurrentShaderProgram() {
    return currentShaderProgram;
}

void GPUImageOpenGLESContext::release() {

    if (context_ == EGL_NO_CONTEXT)
        return;

    eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);   
    eglDestroySurface(display_, surface_);   
    eglDestroyContext(display_, context_);   
    eglTerminate(display_); 

    context_ = EGL_NO_CONTEXT;
}

GPUImageOpenGLESContext::GPUImageOpenGLESContext() 
    : context_(EGL_NO_CONTEXT) {

}

EGLBoolean GPUImageOpenGLESContext::initializeContext(EGLNativeDisplayType nativeDisplay/*, EGLNativeWindowType nativeWindow*/) {

    EGLint contextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

    // Get Display
    display_ = eglGetDisplay(nativeDisplay);
    if (display_ == EGL_NO_DISPLAY) {
        return EGL_FALSE;
    }

    // Initialize EGL
    EGLint majorVersion;
    EGLint minorVersion;
    if (!eglInitialize(display_, &majorVersion, &minorVersion)) {
        return EGL_FALSE;
    }

    // Get configs
    EGLint numConfigs;
    if (!eglGetConfigs(display_, EGL_FALSE, 0, &numConfigs)) {
        return EGL_FALSE;
    }

    // Choose config
    EGLConfig config;
    EGLint wantedConfiguration[] =
    {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    if (!eglChooseConfig(display_, wantedConfiguration, &config, 1, &numConfigs)) {
        return EGL_FALSE;
    }
    
    /*
    // Create a surface
    surface_ = eglCreateWindowSurface(display_, config, nativeWindow, EGL_FALSE);
    if ( surface_ == EGL_NO_SURFACE ) {
        return EGL_FALSE;
    }*/

    // Create a surface
    EGLint surfaceAttributes[] = {   
        EGL_WIDTH,  240,   
        EGL_HEIGHT, 320,   
        EGL_NONE   
    };   
    surface_ = eglCreatePbufferSurface(display_, config, surfaceAttributes);
    if (surface_ == EGL_NO_SURFACE) {
        return EGL_FALSE;
    }

    // Create a GL context
    context_ = eglCreateContext(display_, config, EGL_NO_CONTEXT, contextAttributes);
    if (context_ == EGL_NO_CONTEXT) {
        return EGL_FALSE;
    }

    // Make the context current
    if (!eglMakeCurrent(display_, surface_, surface_, context_)) {
        return EGL_FALSE;
    }

    // Set up a few global settings for the image processing pipeline
    glDisable(GL_DEPTH_TEST);

    return EGL_TRUE;
}

void GPUImageOpenGLESContext::setCurrentShaderProgram(GLProgram* shaderProgram) {
    currentShaderProgram = shaderProgram;
}