/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */
 
#ifndef _GPUImageOpenGLESContext_h_
#define _GPUImageOpenGLESContext_h_

#include "Platform.h"
#include "Common.h"
#include "GPUImageOpenGL.h"
//#include <EGL/egl.h>
#include <string>

// TODO: These need to be defined in a cross platform way
//EGLBoolean
//EGLDisplay
//EGLSurface
//EGLContext

// Forward Declarations
class GLProgram;

// Singleton implemented following this http://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class GPUImageOpenGLESContext {
public:

    static GPUImageOpenGLESContext& sharedImageProcessingOpenGLESContext();
    //TODO: static dispatch_queue_t sharedOpenGLESQueue();
    static void useImageProcessingContext();
    static void setActiveShaderProgram(GLProgram* shaderProgram);
    static GLint maximumTextureSizeForThisDevice();
    static GLint maximumTextureUnitsForThisDevice();
    static gpu_float_size sizeThatFitsWithinATextureForSize(const gpu_float_size& inputSize);

    //TODO: void presentBufferForDisplay();
    GLProgram* programForVertexShaderString(const std::string& vertexShaderString, const std::string& fragmentShaderString);

    // Manage fast texture upload (iOS 5.0 specific?)
    // TODO: static bool supportsFastTextureUpload();
    
    EGLContext getContext();
    GLProgram* getCurrentShaderProgram();
    void release();
    
private:
    GPUImageOpenGLESContext();
    GPUImageOpenGLESContext(GPUImageOpenGLESContext const&);    // Don't Implement
    void operator=(GPUImageOpenGLESContext const&);             // Don't implement
    
    EGLBoolean initializeContext(EGLNativeDisplayType nativeDisplay/*, EGLNativeWindowType nativeWindow*/);
    void makeCurrent();
    void setCurrentShaderProgram(GLProgram* shaderProgram);

    EGLContext context_;
    EGLDisplay display_;
    EGLSurface surface_;

    GLProgram*  currentShaderProgram_;
};

#endif // _GPUImageOpenGLESContext_h_
