/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */
 
#ifndef _PNGImageContainer_h_
#define _PNGImageContainer_h_
 
#include "ImageContainer.h"
#include <string>

class PNGImageContainer : public ImageContainer {
public:
    PNGImageContainer();
    PNGImageContainer(const std::string& fileName);
    ~PNGImageContainer();
    
    bool load(const std::string& fileName);

    gpu_int getWidth() const;
    gpu_int getHeight() const;
    void getRawBytes(char* destination);

private:
};
 
#endif // _PNGImageContainer_h_