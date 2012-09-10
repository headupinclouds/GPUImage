/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */
 
 #ifndef _GPUImageInput_h_
 #define _GPUImageInput_h_
 
 #include "Platform.h"
 #include "Common.h"

 class GPUImageInput {
 public:
    virtual void newFrameReadyAtTime(gpu_time frameTime, gpu_int textureIndex) = 0;
    virtual void setInputTexture(GLuint newInputTexture, gpu_int textureIndex) = 0;
    virtual gpu_int nextAvailableTextureIndex() = 0;
    virtual void setInputSize(gpu_float_size newSize, gpu_int textureIndex) = 0;
    virtual void setInputRotation(GPUImageRotationMode newInputRotation, gpu_int textureIndex) = 0;
    virtual gpu_float_size maximumOutputSize() = 0;
    virtual void endProcessing() = 0;
    virtual bool shouldIgnoreUpdatesToThisTarget() = 0;
    virtual bool enabled () = 0;
};

#endif // _GPUImageInput_h_