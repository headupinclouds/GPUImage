#include "GPUImageOutput.h"
#include "GPUImageInput.h"
#include "GPUImageOpenGLESContext.h"
#include <algorithm>

GPUImageOutput::GPUImageOutput() 
    : outputTexture_(0), overrideInputSize_(false), targetToIgnoreForUpdates_(NULL) {

    forcedMaximumSize_.width = 0.0f;
    forcedMaximumSize_.height = 0.0f;

    inputTextureSize_.width = 0.0f;
    inputTextureSize_.height = 0.0f;

    cachedMaximumOutputSize_.width = 0.0f;
    cachedMaximumOutputSize_.height = 0.0f;

    initializeOutputTexture();
}

GPUImageOutput::~GPUImageOutput() {
    destroy();
}

void GPUImageOutput::setInputTextureForTarget(GPUImageInput* target, gpu_int inputTextureIndex) {
    if (target)
        target->setInputTexture(textureForOutput(), inputTextureIndex);
}

GLuint GPUImageOutput::textureForOutput() {
    return outputTexture_;
}

void GPUImageOutput::notifyTargetsAboutNewOutputTexture() {

    size_t numberTargets = targets_.size();

    for (size_t k = 0; k < numberTargets; k++) {

        GPUImageInput* currentTarget = targets_[k];
        gpu_int textureIndex = targetTextureIndices_[k];
        
        setInputTextureForTarget(currentTarget, textureIndex);
    }
}

void GPUImageOutput::getTargets(std::vector<GPUImageInput*>& targetsVector) {
    targetsVector = targets_;
}

void GPUImageOutput::addTarget(GPUImageInput* newTarget) {
    
    gpu_int nextAvailableTextureIndex = newTarget->nextAvailableTextureIndex();

    addTarget(newTarget, nextAvailableTextureIndex);
    if (newTarget->shouldIgnoreUpdatesToThisTarget()) {
        targetToIgnoreForUpdates_ = newTarget;
    }    
}

void GPUImageOutput::addTarget(GPUImageInput* newTarget, gpu_int textureLocation) {
    
    std::vector<GPUImageInput*>::const_iterator found = std::find(targets_.begin(), targets_.end(), newTarget); 
    if(found != targets_.end()) {
        return;
    }
    
    cachedMaximumOutputSize_.width = cachedMaximumOutputSize_.height = 0.0f;

    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
        [self setInputTextureForTarget:newTarget atIndex:textureLocation];
        [targets addObject:newTarget];
        [targetTextureIndices addObject:[NSNumber numberWithInteger:textureLocation]];
    });*/
    setInputTextureForTarget(newTarget, textureLocation);
    targets_.push_back(newTarget);
    targetTextureIndices_.push_back(textureLocation);
    // End runSynchronouslyOnVideoProcessingQueue block
}

void GPUImageOutput::removeTarget(GPUImageInput* targetToRemove) {

    std::vector<GPUImageInput*>::const_iterator found = std::find(targets_.begin(), targets_.end(), targetToRemove); 
    if(found == targets_.end()) {
        return;
    }

    if (targetToIgnoreForUpdates_ == targetToRemove) {
        targetToIgnoreForUpdates_ = NULL;
    }

    cachedMaximumOutputSize_.width = cachedMaximumOutputSize_.height = 0.0f;

    gpu_int indexOfObject = (found - targets_.begin());
    gpu_int textureIndexOfTarget = targetTextureIndices_[indexOfObject];

    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
    [targetToRemove setInputSize:CGSizeZero atIndex:textureIndexOfTarget];
    [targetToRemove setInputTexture:0 atIndex:textureIndexOfTarget];

    [targetTextureIndices removeObjectAtIndex:indexOfObject];
    [targets removeObject:targetToRemove];
    [targetToRemove endProcessing];
    });*/

    // cachedMaximumOutputSize_ is initialized to 0.0f/0.0f
    targetToRemove->setInputSize(cachedMaximumOutputSize_, textureIndexOfTarget);
    targetToRemove->setInputTexture(0, textureIndexOfTarget);
    targets_.erase(found);
    targetToRemove->endProcessing();
    // End runSynchronouslyOnVideoProcessingQueue block
}

void GPUImageOutput::removeAllTargets() {

    cachedMaximumOutputSize_.width = cachedMaximumOutputSize_.height = 0.0f;

    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
    for (id<GPUImageInput> targetToRemove in targets)
    {
    NSInteger indexOfObject = [targets indexOfObject:targetToRemove];
    NSInteger textureIndexOfTarget = [[targetTextureIndices objectAtIndex:indexOfObject] integerValue];

    [targetToRemove setInputSize:CGSizeZero atIndex:textureIndexOfTarget];
    [targetToRemove setInputTexture:0 atIndex:[[targetTextureIndices objectAtIndex:indexOfObject] integerValue]];
    [targetToRemove setInputRotation:kGPUImageNoRotation atIndex:textureIndexOfTarget];
    }
    [targets removeAllObjects];
    [targetTextureIndices removeAllObjects];
    });*/
    size_t numberTargets = targets_.size();

    for (size_t k = 0; k < numberTargets; k++) {
        GPUImageInput* targetToRemove = targets_[k];

        gpu_int indexOfObject = k;
        gpu_int textureIndexOfTarget = targetTextureIndices_[indexOfObject];

        targetToRemove->setInputSize(cachedMaximumOutputSize_, textureIndexOfTarget);
        targetToRemove->setInputTexture(0, textureIndexOfTarget);
        targetToRemove->setInputRotation(kGPUImageNoRotation, textureIndexOfTarget);
    }

    targets_.clear();
    targetTextureIndices_.clear();
    // End runSynchronouslyOnVideoProcessingQueue block
}
void GPUImageOutput::initializeOutputTexture() {
    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
        [GPUImageOpenGLESContext useImageProcessingContext];

        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &outputTexture);
        glBindTexture(GL_TEXTURE_2D, outputTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // This is necessary for non-power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
    });
    */
    GPUImageOpenGLESContext::useImageProcessingContext();
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &outputTexture_);
    glBindTexture(GL_TEXTURE_2D, outputTexture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // This is necessary for non-power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    // End runSynchronouslyOnVideoProcessingQueue block
}

void GPUImageOutput::deleteOutputTexture() {
    // Note that this run... was already commented in the original file
    //    runSynchronouslyOnVideoProcessingQueue(^{
    //        [GPUImageOpenGLESContext useImageProcessingContext];

    if (outputTexture_) {
        glDeleteTextures(1, &outputTexture_);
        outputTexture_ = 0;
    }
    //    });
}

void GPUImageOutput::forceProcessingAtSize(gpu_float_size frameSize) {

}

void GPUImageOutput::forceProcessingAtSizeRespectingAspectRatio(gpu_float_size frameSize) {

}

bool GPUImageOutput::shouldSmoothlyScaleOutput() {
    return shouldSmoothlyScaleOutput_;
}

void GPUImageOutput::setShouldSmoothlyScaleOutput(bool scale) {
    shouldSmoothlyScaleOutput_ = scale;
}

bool GPUImageOutput::shouldIgnoreUpdatesToThisTarget() {
    return shouldIgnoreUpdatesToThisTarget_;
}

void GPUImageOutput::setShouldIgnoreUpdatesToThisTarget(bool ignore) {
    shouldIgnoreUpdatesToThisTarget_ = ignore;
}

GPUImageInput* GPUImageOutput::getTargetToIgnoreForUpdates() {
    return targetToIgnoreForUpdates_;
}

void GPUImageOutput::setTargetToIgnoreForUpdates(GPUImageInput* target) {
    targetToIgnoreForUpdates_ = target;
}

void GPUImageOutput::destroy() {
    removeAllTargets();
    deleteOutputTexture();
}