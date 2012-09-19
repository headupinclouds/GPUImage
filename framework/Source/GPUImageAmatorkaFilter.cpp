/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImageAmatorkaFilter.h"
#include "GPUImagePicture.h"
#include "GPUImageLookupFilter.h"
#include "PNGImageContainer.h"

GPUImageAmatorkaFilter::GPUImageAmatorkaFilter() :
    GPUImageFilterGroup(),
    image_(NULL),
    lookupImageSource_(NULL) 
{

    image_ = new PNGImageContainer();
    if (!image_) {
        // TODO: log error
        return;
    }

    if (!image_->load("lookup_amatorka.png")) {
        //TODO: NSAssert(image, @"To use GPUImageAmatorkaFilter you need to add lookup_amatorka.png from GPUImage/framework/Resources to your application bundle.");
        delete image_;
        return;
    }

    
    lookupImageSource_ = new GPUImagePicture();
    if (!lookupImageSource_) {
        // TODO: log error
        return;
    }

    lookupImageSource_->initialize(image_);
    
    GPUImageLookupFilter* lookupFilter = new GPUImageLookupFilter();
    if (!lookupFilter) {
        // TODO: log error
        delete lookupImageSource_;
        delete image_;
        return;
    }

    lookupImageSource_->addTarget(lookupFilter, 1);
    lookupImageSource_->processImage();

    initialFilters_.push_back(lookupFilter);
    terminalFilter_ = lookupFilter;
}

GPUImageAmatorkaFilter::~GPUImageAmatorkaFilter() {

}