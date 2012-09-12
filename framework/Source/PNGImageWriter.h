/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _PNGImageWriter_h_
#define _PNGImageWriter_h_

#include "GPUImageInput.h"

class PNGImageWriter : public GPUImageInput {
public:
	PNGImageWriter();
	~PNGImageWriter();

    void newFrameReadyAtTime(gpu_time frameTime, gpu_int textureIndex);
    void setInputTexture(GLuint newInputTexture, gpu_int textureIndex);
    gpu_int nextAvailableTextureIndex();
    void setInputSize(gpu_float_size newSize, gpu_int textureIndex);
    void setInputRotation(GPUImageRotationMode newInputRotation, gpu_int textureIndex);
    gpu_float_size maximumOutputSize();
    void endProcessing();
    bool shouldIgnoreUpdatesToThisTarget();
    bool enabled();
};

#endif // _PNGImageWriter_h_