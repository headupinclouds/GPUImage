# This file generated automatically by:
#   generate_sugar_files.py
# see wiki for more info:
#   https://github.com/ruslo/sugar/wiki/Collecting-sources

if(DEFINED FRAMEWORK_SOURCE_SUGAR_CMAKE_)
  return()
else()
  set(FRAMEWORK_SOURCE_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)

sugar_files(
    GPUIMAGE_FRAMEWORK_SRCS
    GLProgram.cpp
    GPUImage3x3ConvolutionFilter.cpp
    GPUImage3x3TextureSamplingFilter.cpp
    GPUImageAdaptiveThresholdFilter.cpp
    GPUImageAddBlendFilter.cpp
    GPUImageAlphaBlendFilter.cpp
    GPUImageAmatorkaFilter.cpp
    GPUImageAverageColor.cpp
    GPUImageBoxBlurFilter.cpp
    GPUImageFilter.cpp
    GPUImageFilterGroup.cpp
    GPUImageGrayscaleFilter.cpp
    GPUImageLookupFilter.cpp
    GPUImageMaskFilter.cpp
    GPUImageOpenGLESContext.cpp
    GPUImageOutput.cpp
    GPUImagePicture.cpp
    GPUImageTwoInputFilter.cpp
    GPUImageTwoPassFilter.cpp
    GPUImageTwoPassTextureSamplingFilter.cpp

# This stuff needs the FreeImage package
#    PNGImageContainer.cpp
#    PNGImageWriter.cpp
)

sugar_files(
    GPUIMAGE_FRAMEWORK_PUBLIC_HDRS
    Common.h
    GLProgram.h
    GPUImage3x3ConvolutionFilter.h
    GPUImage3x3TextureSamplingFilter.h
    GPUImageAdaptiveThresholdFilter.h
    GPUImageAddBlendFilter.h
    GPUImageAlphaBlendFilter.h
    GPUImageAmatorkaFilter.h
    GPUImageAverageColor.h
    GPUImageBoxBlurFilter.h
    GPUImageFilter.h
    GPUImageFilterGroup.h
    GPUImageGrayscaleFilter.h
    GPUImageInput.h
    GPUImageLookupFilter.h
    GPUImageMaskFilter.h
    GPUImageOpenGLESContext.h
    GPUImageOutput.h
    GPUImagePicture.h
    GPUImageTwoInputFilter.h
    GPUImageTwoPassFilter.h
    GPUImageTwoPassTextureSamplingFilter.h
    Platform.h

# This stuff needs the FreeImage package
#    PNGImageContainer.h
#    PNGImageWriter.h
)
