/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "PNGImageContainer.h"
#include <GLES2/gl2.h>
#include "FreeImage.h"

PNGImageContainer::PNGImageContainer() 
    : width_(0), height_(0), bitsPerPixel_(0), imageFormat_(0), data_(NULL), dib_(NULL) {

}

PNGImageContainer::PNGImageContainer(const std::string& fileName) 
    : width_(0), height_(0), bitsPerPixel_(0), imageFormat_(0), data_(NULL), dib_(NULL) {
    
    load(fileName);
}

PNGImageContainer::~PNGImageContainer() {
    destroy();
}

bool PNGImageContainer::load(const std::string& fileName) {

    // some sanity checks
    if (fileName.length() == 0)
        return false;

    if (data_)
        destroy();

    // we keep the DIB in memory to allow the resize
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str());
    dib_ = FreeImage_Load(format, fileName.c_str());
    FREE_IMAGE_COLOR_TYPE type = FreeImage_GetColorType(dib_);

    width_ = FreeImage_GetWidth(dib_);
    height_ = FreeImage_GetHeight(dib_);
    bitsPerPixel_ = FreeImage_GetBPP(dib_) / 8;


    switch(type) {
        case FIC_RGBALPHA:
            imageFormat_ = GL_RGBA;
            break;
        case FIC_RGB:
            imageFormat_ = GL_RGB;
            break;
    }

    size_t bufferSize = width_ * height_ * bitsPerPixel_;

    BYTE* imageData = FreeImage_GetBits(dib_);
    data_ = new BYTE[bufferSize];

    memcpy(data_, imageData, bufferSize);
    
    return true;
}

gpu_int PNGImageContainer::getWidth() const {
    return width_;
}

gpu_int PNGImageContainer::getHeight() const {
    return height_;
}

gpu_int PNGImageContainer::getBitsPerPixel() const {
    return bitsPerPixel_;
}

gpu_int PNGImageContainer::getFormat() const {
    return imageFormat_;
}

void PNGImageContainer::resize(gpu_int width, gpu_int height) {

    FIBITMAP* resizedDib = FreeImage_Rescale(dib_, width, height, FILTER_BILINEAR);
    if (resizedDib == NULL)
        return;

    // delete old stuff
    destroy();

    // update with new stuff
    dib_ = resizedDib;
    width_ = width;
    height_ = height;

    // extract image bits
    size_t bufferSize = width_ * height_ * bitsPerPixel_;
    BYTE* imageData = FreeImage_GetBits(dib_);
    data_ = new BYTE[bufferSize];

    memcpy(data_, imageData, bufferSize);
}

unsigned char* PNGImageContainer::getRawBytes() const  {
    return data_;
}

void PNGImageContainer::destroy() {
    if (data_)
        delete [] data_;

    data_ = NULL;

    if (dib_)
        FreeImage_Unload(dib_);

    dib_ = NULL;
}