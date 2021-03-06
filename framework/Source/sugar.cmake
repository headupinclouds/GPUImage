# This file generated automatically by:
#   generate_sugar_files.py
# see wiki for more info:
#   https://github.com/ruslo/sugar/wiki/Collecting-sources

if(DEFINED GPUIMAGE_SOURCE_SUGAR_CMAKE_)
  return()
else()
  set(GPUIMAGE_SOURCE_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)
include(sugar_include)

string(COMPARE EQUAL "${CMAKE_OSX_SYSROOT}" "iphoneos" is_ios)
if(is_ios)
  sugar_include(iOS)
  sugar_files(
      GPUIMAGE_SOURCES
      GPUImageFilterPipeline.h
      GPUImageFilterPipeline.m
      GPUImageMovie.h
      GPUImageMovie.m
      GPUImageMovieComposition.h
      GPUImageMovieComposition.m
      GPUImageStillCamera.h
      GPUImageStillCamera.m
      GPUImageUIElement.h
      GPUImageUIElement.m
      GPUImageVideoCamera.h
      GPUImageVideoCamera.m
  )
else()
  sugar_include(Mac)
endif()

sugar_files(
    GPUIMAGE_SOURCES
    GLProgram.h
    GLProgram.m
    GPUImage.h
    GPUImageFramebuffer.h
    GPUImageFramebuffer.m
    GPUImageFramebufferCache.h
    GPUImageFramebufferCache.m
    GPUImage3x3ConvolutionFilter.h
    GPUImage3x3ConvolutionFilter.m
    GPUImage3x3TextureSamplingFilter.h
    GPUImage3x3TextureSamplingFilter.m
    GPUImageAdaptiveThresholdFilter.h
    GPUImageAdaptiveThresholdFilter.m
    GPUImageAddBlendFilter.h
    GPUImageAddBlendFilter.m
    GPUImageAlphaBlendFilter.h
    GPUImageAlphaBlendFilter.m
    GPUImageAmatorkaFilter.h
    GPUImageAmatorkaFilter.m
    GPUImageAverageColor.h
    GPUImageAverageColor.m
    GPUImageAverageLuminanceThresholdFilter.h
    GPUImageAverageLuminanceThresholdFilter.m
    GPUImageBilateralFilter.h
    GPUImageBilateralFilter.m
    GPUImageBoxBlurFilter.h
    GPUImageBoxBlurFilter.m
    GPUImageBrightnessFilter.h
    GPUImageBrightnessFilter.m
    GPUImageBuffer.h
    GPUImageBuffer.m
    GPUImageBulgeDistortionFilter.h
    GPUImageBulgeDistortionFilter.m
    GPUImageCannyEdgeDetectionFilter.h
    GPUImageCannyEdgeDetectionFilter.m
    GPUImageCGAColorspaceFilter.h
    GPUImageCGAColorspaceFilter.m
    GPUImageChromaKeyBlendFilter.h
    GPUImageChromaKeyBlendFilter.m
    GPUImageChromaKeyFilter.h
    GPUImageChromaKeyFilter.m
    GPUImageClosingFilter.h
    GPUImageClosingFilter.m
    GPUImageColorBlendFilter.h
    GPUImageColorBlendFilter.m
    GPUImageColorBurnBlendFilter.h
    GPUImageColorBurnBlendFilter.m
    GPUImageColorDodgeBlendFilter.h
    GPUImageColorDodgeBlendFilter.m
    GPUImageColorInvertFilter.h
    GPUImageColorInvertFilter.m
    GPUImageColorMatrixFilter.h
    GPUImageColorMatrixFilter.m
    GPUImageColorPackingFilter.h
    GPUImageColorPackingFilter.m
    GPUImageContrastFilter.h
    GPUImageContrastFilter.m
    GPUImageCropFilter.h
    GPUImageCropFilter.m
    GPUImageCrosshairGenerator.h
    GPUImageCrosshairGenerator.m
    GPUImageCrosshatchFilter.h
    GPUImageCrosshatchFilter.m
    GPUImageDarkenBlendFilter.h
    GPUImageDarkenBlendFilter.m
    GPUImageDifferenceBlendFilter.h
    GPUImageDifferenceBlendFilter.m
    GPUImageDilationFilter.h
    GPUImageDilationFilter.m
    GPUImageDirectionalNonMaximumSuppressionFilter.h
    GPUImageDirectionalNonMaximumSuppressionFilter.m
    GPUimageDirectionalSobelEdgeDetectionFilter.h
    GPUimageDirectionalSobelEdgeDetectionFilter.m
    GPUImageDissolveBlendFilter.h
    GPUImageDissolveBlendFilter.m
    GPUImageDivideBlendFilter.h
    GPUImageDivideBlendFilter.m
    GPUImageEmbossFilter.h
    GPUImageEmbossFilter.m
    GPUImageErosionFilter.h
    GPUImageErosionFilter.m
    GPUImageExclusionBlendFilter.h
    GPUImageExclusionBlendFilter.m
    GPUImageExposureFilter.h
    GPUImageExposureFilter.m
    GPUImageFalseColorFilter.h
    GPUImageFalseColorFilter.m
    GPUImageFASTCornerDetectionFilter.h
    GPUImageFASTCornerDetectionFilter.m
    GPUImageFilter.h
    GPUImageFilter.m
    GPUImageFilterGroup.h
    GPUImageFilterGroup.m
    GPUImageGammaFilter.h
    GPUImageGammaFilter.m
    GPUImageGaussianBlurFilter.h
    GPUImageGaussianBlurFilter.m
    GPUImageGaussianBlurPositionFilter.h
    GPUImageGaussianBlurPositionFilter.m
    GPUImageGaussianSelectiveBlurFilter.h
    GPUImageGaussianSelectiveBlurFilter.m
    GPUImageGlassSphereFilter.h
    GPUImageGlassSphereFilter.m
    GPUImageGrayscaleFilter.h
    GPUImageGrayscaleFilter.m
    GPUImageHalftoneFilter.h
    GPUImageHalftoneFilter.m
    GPUImageHardLightBlendFilter.h
    GPUImageHardLightBlendFilter.m
    GPUImageHarrisCornerDetectionFilter.h
    GPUImageHarrisCornerDetectionFilter.m
    GPUImageHazeFilter.h
    GPUImageHazeFilter.m
    GPUImageHighlightShadowFilter.h
    GPUImageHighlightShadowFilter.m
    GPUImageHighPassFilter.h
    GPUImageHighPassFilter.m
    GPUImageHistogramEqualizationFilter.h
    GPUImageHistogramEqualizationFilter.m
    GPUImageHistogramFilter.h
    GPUImageHistogramFilter.m
    GPUImageHistogramGenerator.h
    GPUImageHistogramGenerator.m
    GPUImageHoughTransformLineDetector.h
    GPUImageHoughTransformLineDetector.m
    GPUImageHSBFilter.h
    GPUImageHSBFilter.m
    GPUImageHueBlendFilter.h
    GPUImageHueBlendFilter.m
    GPUImageHueFilter.h
    GPUImageHueFilter.m
    GPUImageiOSBlurFilter.h
    GPUImageiOSBlurFilter.m
    GPUImageJFAVoronoiFilter.h
    GPUImageJFAVoronoiFilter.m
    GPUImageKuwaharaFilter.h
    GPUImageKuwaharaFilter.m
    GPUImageKuwaharaRadius3Filter.h
    GPUImageKuwaharaRadius3Filter.m
    GPUImageLanczosResamplingFilter.h
    GPUImageLanczosResamplingFilter.m
    GPUImageLaplacianFilter.h
    GPUImageLaplacianFilter.m
    GPUImageLevelsFilter.h
    GPUImageLevelsFilter.m
    GPUImageLightenBlendFilter.h
    GPUImageLightenBlendFilter.m
    GPUImageLinearBurnBlendFilter.h
    GPUImageLinearBurnBlendFilter.m
    GPUImageLineGenerator.h
    GPUImageLineGenerator.m
    GPUImageLocalBinaryPatternFilter.h
    GPUImageLocalBinaryPatternFilter.m
    GPUImageLookupFilter.h
    GPUImageLookupFilter.m
    GPUImageLowPassFilter.h
    GPUImageLowPassFilter.m
    GPUImageLuminanceRangeFilter.h
    GPUImageLuminanceRangeFilter.m
    GPUImageLuminanceThresholdFilter.h
    GPUImageLuminanceThresholdFilter.m
    GPUImageLuminosity.h
    GPUImageLuminosity.m
    GPUImageLuminosityBlendFilter.h
    GPUImageLuminosityBlendFilter.m
    GPUImageMaskFilter.h
    GPUImageMaskFilter.m
    GPUImageMedianFilter.h
    GPUImageMedianFilter.m
    GPUImageMissEtikateFilter.h
    GPUImageMissEtikateFilter.m
    GPUImageMonochromeFilter.h
    GPUImageMonochromeFilter.m
    GPUImageMosaicFilter.h
    GPUImageMosaicFilter.m
    GPUImageMotionBlurFilter.h
    GPUImageMotionBlurFilter.m
    GPUImageMotionDetector.h
    GPUImageMotionDetector.m
    GPUImageMultiplyBlendFilter.h
    GPUImageMultiplyBlendFilter.m
    GPUImageNobleCornerDetectionFilter.h
    GPUImageNobleCornerDetectionFilter.m
    GPUImageNonMaximumSuppressionFilter.h
    GPUImageNonMaximumSuppressionFilter.m
    GPUImageNormalBlendFilter.h
    GPUImageNormalBlendFilter.m
    GPUImageOpacityFilter.h
    GPUImageOpacityFilter.m
    GPUImageOpeningFilter.h
    GPUImageOpeningFilter.m
    GPUImageOutput.h
    GPUImageOutput.m
    GPUImageOverlayBlendFilter.h
    GPUImageOverlayBlendFilter.m
    GPUImageParallelCoordinateLineTransformFilter.h
    GPUImageParallelCoordinateLineTransformFilter.m
    GPUImagePerlinNoiseFilter.h
    GPUImagePerlinNoiseFilter.m
    GPUImagePinchDistortionFilter.h
    GPUImagePinchDistortionFilter.m
    GPUImagePixellateFilter.h
    GPUImagePixellateFilter.m
    GPUImagePixellatePositionFilter.h
    GPUImagePixellatePositionFilter.m
    GPUImagePoissonBlendFilter.h
    GPUImagePoissonBlendFilter.m
    GPUImagePolarPixellateFilter.h
    GPUImagePolarPixellateFilter.m
    GPUImagePolkaDotFilter.h
    GPUImagePolkaDotFilter.m
    GPUImagePosterizeFilter.h
    GPUImagePosterizeFilter.m
    GPUImagePrewittEdgeDetectionFilter.h
    GPUImagePrewittEdgeDetectionFilter.m
    GPUImageRawDataInput.h
    GPUImageRawDataInput.m
    GPUImageRawDataOutput.h
    GPUImageRawDataOutput.m
    GPUImageRGBClosingFilter.h
    GPUImageRGBClosingFilter.m
    GPUImageRGBDilationFilter.h
    GPUImageRGBDilationFilter.m
    GPUImageRGBErosionFilter.h
    GPUImageRGBErosionFilter.m
    GPUImageRGBFilter.h
    GPUImageRGBFilter.m
    GPUImageRGBOpeningFilter.h
    GPUImageRGBOpeningFilter.m
    GPUImageSaturationBlendFilter.h
    GPUImageSaturationBlendFilter.m
    GPUImageSaturationFilter.h
    GPUImageSaturationFilter.m
    GPUImageScreenBlendFilter.h
    GPUImageScreenBlendFilter.m
    GPUImageSepiaFilter.h
    GPUImageSepiaFilter.m
    GPUImageSharpenFilter.h
    GPUImageSharpenFilter.m
    GPUImageShiTomasiFeatureDetectionFilter.h
    GPUImageShiTomasiFeatureDetectionFilter.m
    GPUImageSingleComponentGaussianBlurFilter.h
    GPUImageSingleComponentGaussianBlurFilter.m
    GPUImageSketchFilter.h
    GPUImageSketchFilter.m
    GPUImageSmoothToonFilter.h
    GPUImageSmoothToonFilter.m
    GPUImageSobelEdgeDetectionFilter.h
    GPUImageSobelEdgeDetectionFilter.m
    GPUImageSoftEleganceFilter.h
    GPUImageSoftEleganceFilter.m
    GPUImageSoftLightBlendFilter.h
    GPUImageSoftLightBlendFilter.m
    GPUImageSolidColorGenerator.h
    GPUImageSolidColorGenerator.m
    GPUImageSourceOverBlendFilter.h
    GPUImageSourceOverBlendFilter.m
    GPUImageSphereRefractionFilter.h
    GPUImageSphereRefractionFilter.m
    GPUImageStretchDistortionFilter.h
    GPUImageStretchDistortionFilter.m
    GPUImageSubtractBlendFilter.h
    GPUImageSubtractBlendFilter.m
    GPUImageSwirlFilter.h
    GPUImageSwirlFilter.m
    GPUImageTextureInput.h
    GPUImageTextureInput.m
    GPUImageTextureOutput.h
    GPUImageTextureOutput.m
    GPUImageThreeInputFilter.h
    GPUImageThreeInputFilter.m
    GPUImageThresholdEdgeDetectionFilter.h
    GPUImageThresholdEdgeDetectionFilter.m
    GPUImageThresholdedNonMaximumSuppressionFilter.h
    GPUImageThresholdedNonMaximumSuppressionFilter.m
    GPUImageThresholdSketchFilter.h
    GPUImageThresholdSketchFilter.m
    GPUImageTiltShiftFilter.h
    GPUImageTiltShiftFilter.m
    GPUImageToneCurveFilter.h
    GPUImageToneCurveFilter.m
    GPUImageToonFilter.h
    GPUImageToonFilter.m
    GPUImageTransformFilter.h
    GPUImageTransformFilter.m
    GPUImageTwoInputCrossTextureSamplingFilter.h
    GPUImageTwoInputCrossTextureSamplingFilter.m
    GPUImageTwoInputFilter.h
    GPUImageTwoInputFilter.m
    GPUImageTwoPassFilter.h
    GPUImageTwoPassFilter.m
    GPUImageTwoPassTextureSamplingFilter.h
    GPUImageTwoPassTextureSamplingFilter.m
    GPUImageUnsharpMaskFilter.h
    GPUImageUnsharpMaskFilter.m
    GPUImageVignetteFilter.h
    GPUImageVignetteFilter.m
    GPUImageVoronoiConsumerFilter.h
    GPUImageVoronoiConsumerFilter.m
    GPUImageWeakPixelInclusionFilter.h
    GPUImageWeakPixelInclusionFilter.m
    GPUImageWhiteBalanceFilter.h
    GPUImageWhiteBalanceFilter.m
    GPUImageXYDerivativeFilter.h
    GPUImageXYDerivativeFilter.m
    GPUImageZoomBlurFilter.h
    GPUImageZoomBlurFilter.m
)
