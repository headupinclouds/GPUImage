#include "GPUImageOutput.h"
#include "GPUImageInput.h"
#include <algorithm>

GPUImageOutput::GPUImageOutput() {

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
    
    cachedMaximumOutputSize.width = cachedMaximumOutputSize.height = 0.0f;

    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    /*runSynchronouslyOnVideoProcessingQueue(^{
        [self setInputTextureForTarget:newTarget atIndex:textureLocation];
        [targets addObject:newTarget];
        [targetTextureIndices addObject:[NSNumber numberWithInteger:textureLocation]];
    });*/
}

void GPUImageOutput::removeTarget(GPUImageInput* targetToRemove) {

}

void GPUImageOutput::removeAllTargets() {

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