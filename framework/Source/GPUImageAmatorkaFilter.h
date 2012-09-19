/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageAmatorkaFilter_h_
#define _GPUImageAmatorkaFilter_h_

#include "GPUImageFilterGroup.h"

// forward declarations
class PNGImageContainer;
class GPUImagePicture;

/** A photo filter based on Photoshop action by Amatorka
    http://amatorka.deviantart.com/art/Amatorka-Action-2-121069631
 */

// Note: If you want to use this effect you have to add lookup_amatorka.png
//       from Resources folder to your application bundle.

class GPUImageAmatorkaFilter : public GPUImageFilterGroup
{
public:
    GPUImageAmatorkaFilter();
    ~GPUImageAmatorkaFilter();

private:
    PNGImageContainer* image_;
    GPUImagePicture* lookupImageSource_;
};

#endif // _GPUImageAmatorkaFilter_h_