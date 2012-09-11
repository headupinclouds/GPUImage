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
    virtual gpu_int getWidth() const;
    virtual gpu_int getHeight() const;
    virtual void getRawBytes(char* destination);
};
 
#endif // _ImageContainer_h_