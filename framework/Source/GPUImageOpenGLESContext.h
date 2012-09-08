/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */
 
#ifndef _GPUImageOpenGLESContext_h_
#define _GPUImageOpenGLESContext_h_

#include "Platform.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#define GPUImageRotationSwapsWidthAndHeight(rotation) ((rotation) == kGPUImageRotateLeft || (rotation) == kGPUImageRotateRight || (rotation) == kGPUImageRotateRightFlipVertical)

typedef enum { 
    kGPUImageNoRotation, 
    kGPUImageRotateLeft, 
    kGPUImageRotateRight, 
    kGPUImageFlipVertical, 
    kGPUImageFlipHorizonal, 
    kGPUImageRotateRightFlipVertical, 
    kGPUImageRotate180 
} GPUImageRotationMode;

// Singleton implemented following this http://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class GPUImageOpenGLESContext {
public:

    static GPUImageOpenGLESContext& sharedImageProcessingOpenGLESContext();
    //TODO: static dispatch_queue_t sharedOpenGLESQueue();
    static void useImageProcessingContext();
    //TODO: static void setActiveShaderProgram(GLProgram* shaderProgram);
    static GLint maximumTextureSizeForThisDevice();
    static GLint maximumTextureUnitsForThisDevice();
    static gpu_float_size sizeThatFitsWithinATextureForSize(const gpu_float_size& inputSize);

    void presentBufferForDisplay();
    //TODO: GLProgram* programForVertexShaderString(const std::string& vertexShaderString, const std::string& fragmentShaderString);

    // Manage fast texture upload
    static bool supportsFastTextureUpload();
    
    EGLContext getContext();
    
private:
/*@property(readonly, retain, nonatomic) EAGLContext *context;
@property(readonly, nonatomic) dispatch_queue_t contextQueue;
@property(readwrite, retain, nonatomic) GLProgram *currentShaderProgram;*/

    GPUImageOpenGLESContext();
    GPUImageOpenGLESContext(GPUImageOpenGLESContext const&);    // Don't Implement
    void operator=(GPUImageOpenGLESContext const&);             // Don't implement
    
    EGLContext context;
};

#endif // _GPUImageOpenGLESContext_h_