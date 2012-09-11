/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */
 
#ifndef _ImageContainer_h_
#define _ImageContainer_h_
 
#include "Platform.h"
#include <string>

class ImageContainer {
public:
    virtual bool load(const std::string& fileName) = 0;
    virtual gpu_int getWidth() const = 0;
    virtual gpu_int getHeight() const = 0;
    virtual gpu_int getBitsPerPixel() const = 0;
    virtual gpu_int getFormat() const = 0;
    virtual void resize(gpu_int width, gpu_int height) = 0;
    virtual unsigned char* getRawBytes() const = 0;
    virtual void destroy() = 0;
};
 
#endif // _ImageContainer_h_