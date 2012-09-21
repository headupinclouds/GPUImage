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
    lookupImageSource_(NULL) 
{

    PNGImageContainer* imageSource = new PNGImageContainer();
    if (!imageSource) {
        // TODO: log error
        return;
    }

    if (!imageSource->load("lookup_amatorka.png")) {
        //TODO: NSAssert(image, @"To use GPUImageAmatorkaFilter you need to add lookup_amatorka.png from GPUImage/framework/Resources to your application bundle.");
        delete imageSource;
        imageSource = NULL;
        return;
    }
    
    lookupImageSource_ = new GPUImagePicture();
    if (!lookupImageSource_) {
        // TODO: log error        
        delete imageSource;
        imageSource = NULL;
        return;
    }

    lookupImageSource_->initialize(imageSource);

    // delete the imageloader since lookupImageSource_ owns a copy of the loaded data buffer
    delete imageSource;
    imageSource = NULL;
    
    GPUImageLookupFilter* lookupFilter = new GPUImageLookupFilter();
    if (!lookupFilter) {
        // TODO: log error
        delete lookupImageSource_;
        return;
    }

    lookupImageSource_->addTarget(lookupFilter, 1);
    lookupImageSource_->processImage();

    initialFilters_.push_back(lookupFilter);
    terminalFilter_ = lookupFilter;
}

GPUImageAmatorkaFilter::~GPUImageAmatorkaFilter() {
    
    //if (
}