/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */
 
#ifndef _ImageContainer_h_
#define _ImageContainer_h_
 
#include "Platform.h"

class ImageContainer {
public:
    virtual gpu_int getWidth() const = 0;
    virtual gpu_int getHeight() const = 0;
    virtual void getRawBytes(char* destination) = 0;
};
 
#endif // _ImageContainer_h_