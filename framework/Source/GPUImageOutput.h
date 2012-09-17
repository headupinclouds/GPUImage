/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageOutput_h_
#define _GPUImageOutput_h_

#include "Platform.h"
#include <GLES2/gl2.h>
#include <vector>

// Forward declarations
class GPUImageInput;

/** GPUImage's base source object
 
 Images or frames of video are uploaded from source objects, which are subclasses of GPUImageOutput. These include:
 
 - GPUImageVideoCamera (for live video from a camera) 
 - GPUImageStillCamera (for taking photos with the camera)
 - GPUImagePicture (for still images)
 - GPUImageMovie (for movies)
 
 Source objects upload still image frames to OpenGL ES as textures, then hand those textures off to the next objects in the processing chain.
 */
class GPUImageOutput {
public:
    GPUImageOutput();
    virtual ~GPUImageOutput();

    void setInputTextureForTarget(GPUImageInput* target, gpu_int inputTextureIndex);
    GLuint textureForOutput();
    void notifyTargetsAboutNewOutputTexture();

    /** Returns an array of the current targets.
     */
    void getTargets(std::vector<GPUImageInput*>& targetsVector);

    /** Adds a target to receive notifications when new frames are available.
     
     The target will be asked for its next available texture.
     
     See [GPUImageInput newFrameReadyAtTime:]
     
     @param newTarget Target to be added
     */
    virtual void addTarget(GPUImageInput* newTarget);

    /** Adds a target to receive notifications when new frames are available.
     
     See [GPUImageInput newFrameReadyAtTime:]
     
     @param newTarget Target to be added
     */
    virtual void addTarget(GPUImageInput* newTarget, gpu_int textureLocation);

    /** Removes a target. The target will no longer receive notifications when new frames are available.
     
     @param targetToRemove Target to be removed
     */
    void removeTarget(GPUImageInput* targetToRemove);

    /** Removes all targets.
     */
    void removeAllTargets();

    /// @name Manage the output texture

    void initializeOutputTexture();
    void deleteOutputTexture();
    void forceProcessingAtSize(gpu_float_size frameSize);
    void forceProcessingAtSizeRespectingAspectRatio(gpu_float_size frameSize);

    // Still images
    virtual void prepareForImageCapture();

    bool shouldSmoothlyScaleOutput();
    void setShouldSmoothlyScaleOutput(bool scale);
    bool shouldIgnoreUpdatesToThisTarget();
    void setShouldIgnoreUpdatesToThisTarget(bool ignore);
    GPUImageInput* getTargetToIgnoreForUpdates();
    virtual void setTargetToIgnoreForUpdates(GPUImageInput* target);

    void destroy();

protected:
    std::vector<GPUImageInput*> targets_;
    std::vector<GLuint> targetTextureIndices_;

    gpu_float_size forcedMaximumSize_;

    GLuint outputTexture_;
    bool overrideInputSize_;
    gpu_float_size inputTextureSize_;
    gpu_float_size cachedMaximumOutputSize_;

    GPUImageInput* targetToIgnoreForUpdates_;

private:
    

    bool shouldSmoothlyScaleOutput_;
    bool shouldIgnoreUpdatesToThisTarget_;
    //TODO: GPUImageMovieWriter *audioEncodingTarget;
    // TODO: void(^frameProcessingCompletionBlock)(GPUImageOutput*, CMTime);
};

#ifdef portcode

/// @name Still image processing

/** Retreives the currently processed image as a UIImage.
 */
- (UIImage *)imageFromCurrentlyProcessedOutput;
- (CGImageRef)newCGImageFromCurrentlyProcessedOutput;

/** Convenience method to retreive the currently processed image with a different orientation.
 @param imageOrientation Orientation for image
 */
- (UIImage *)imageFromCurrentlyProcessedOutputWithOrientation:(UIImageOrientation)imageOrientation;
- (CGImageRef)newCGImageFromCurrentlyProcessedOutputWithOrientation:(UIImageOrientation)imageOrientation;

/** Convenience method to process an image with a filter.
 
 This method is useful for using filters on still images without building a full pipeline.
 
 @param imageToFilter Image to be filtered
 */
- (UIImage *)imageByFilteringImage:(UIImage *)imageToFilter;
- (CGImageRef)newCGImageByFilteringImage:(UIImage *)imageToFilter;
- (CGImageRef)newCGImageByFilteringCGImage:(CGImageRef)imageToFilter;
- (CGImageRef)newCGImageByFilteringCGImage:(CGImageRef)imageToFilter orientation:(UIImageOrientation)orientation;

- (void)prepareForImageCapture;
#endif

#endif // _GPUImageOutput_h_