/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "PNGImageContainer.h"
#include <GLES2/gl2.h>
#include <algorithm>
#include "FreeImage.h"
#include "Common.h"

PNGImageContainer::PNGImageContainer() 
    : width_(0), height_(0), bytesPerPixel_(0), imageFormat_(0), data_(NULL), dib_(NULL) {

}

PNGImageContainer::PNGImageContainer(const std::string& fileName) 
    : width_(0), height_(0), bytesPerPixel_(0), imageFormat_(0), data_(NULL), dib_(NULL) {
    
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
    FIBITMAP* originalImage = FreeImage_Load(format, fileName.c_str());

    // convert to 32 bits
	dib_ = FreeImage_ConvertTo32Bits(originalImage);
	FreeImage_Unload(originalImage);

    FREE_IMAGE_COLOR_TYPE type = FreeImage_GetColorType(dib_);

    width_ = FreeImage_GetWidth(dib_);
    height_ = FreeImage_GetHeight(dib_);
    bytesPerPixel_ = FreeImage_GetBPP(dib_) / 8;

    // TODO: should this always report FIC_RGBALPHA because of the ConvertTo32Bits?
    /*switch(type) {
        case FIC_RGBALPHA:
            imageFormat_ = GL_RGBA;
            break;
        case FIC_RGB:
            imageFormat_ = GL_RGB;
            break;
    }*/
    imageFormat_ = GL_RGBA;

    size_t bufferSize = width_ * height_ * bytesPerPixel_;

    BYTE* imageData = FreeImage_GetBits(dib_);
    BYTE* imagePointer = imageData;

    // Convert the image from BGRA to RGBA using the CPU, since FreeImage loads images to BGR
    // Taken from Conversion.cpp (FreeImage Library), SwapRedBlue32()
	const unsigned pitch = FreeImage_GetPitch(dib_);
	const unsigned lineSize = FreeImage_GetLine(dib_);

	for(unsigned y = 0; y < height_; ++y, imagePointer += pitch) {
		for(BYTE* pixel = imagePointer; pixel < imagePointer + lineSize; pixel += bytesPerPixel_) {
			INPLACESWAP(pixel[0], pixel[2]);
		}
	}   

    data_ = new BYTE[bufferSize];

    // we use std::copy instead of memcpy ( http://stackoverflow.com/questions/4707012/c-memcpy-vs-stdcopy )
    //memcpy(data_, imageData, bufferSize);
    std::copy(imageData, imageData + bufferSize, data_);

    return true;
}

gpu_int PNGImageContainer::getWidth() const {
    return width_;
}

gpu_int PNGImageContainer::getHeight() const {
    return height_;
}

gpu_int PNGImageContainer::getBitsPerPixel() const {
    return bytesPerPixel_;
}

gpu_int PNGImageContainer::getFormat() const {
    return imageFormat_;
}

bool PNGImageContainer::resize(gpu_int width, gpu_int height) {

    FIBITMAP* resizedDib = FreeImage_Rescale(dib_, width, height, FILTER_BILINEAR);
    if (resizedDib == NULL)
        return false;

    // delete old stuff
    destroy();

    // update with new stuff
    dib_ = resizedDib;
    width_ = width;
    height_ = height;

    // extract image bits
    size_t bufferSize = width_ * height_ * bytesPerPixel_;
    BYTE* imageData = FreeImage_GetBits(dib_);
    data_ = new BYTE[bufferSize];

    // we use std::copy instead of memcpy ( http://stackoverflow.com/questions/4707012/c-memcpy-vs-stdcopy )
    //memcpy(data_, imageData, bufferSize);
    std::copy(imageData, imageData + bufferSize, data_);

    return true;
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