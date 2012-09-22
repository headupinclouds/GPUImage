/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageMaskFilter_h_
#define _GPUImageMaskFilter_h_

#include "GPUImageTwoInputFilter.h"
#include <string>

class GPUImageMaskFilter : public GPUImageTwoInputFilter 
{
public:
    GPUImageMaskFilter();
    ~GPUImageMaskFilter();

    void renderToTextureWithVertices(const GLfloat* vertices, const GLfloat* textureCoordinates, GLuint sourceTexture);

    static const std::string kGPUImageMaskShaderString;
};

#endif //_GPUImageMaskFilter_h_
