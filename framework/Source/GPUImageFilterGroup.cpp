/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImageFilterGroup.h"

GPUImageFilterGroup::GPUImageFilterGroup() : 
    GPUImageOutput(),
    shouldIgnoreUpdatesToThisTarget_(false),
    enabled_(false),
    terminalFilter_(NULL), 
    inputFilterToIgnoreForUpdates_(NULL) {

    deleteOutputTexture();
}

GPUImageFilterGroup::~GPUImageFilterGroup() {

}

void GPUImageFilterGroup::addFilter(GPUImageOutput* newFilter) {
    filters_.push_back(newFilter);
}

GPUImageOutput* GPUImageFilterGroup::filterAtIndex(gpu_uint filterIndex) {
    return filters_[filterIndex];
}

int GPUImageFilterGroup::getFilterCount() {
    return filters_.size();
}

/*

- (CGImageRef)newCGImageFromCurrentlyProcessedOutputWithOrientation:(UIImageOrientation)imageOrientation;
{
    return [self.terminalFilter newCGImageFromCurrentlyProcessedOutputWithOrientation:imageOrientation];
}
*/

void GPUImageFilterGroup::prepareForImageCapture() {
    if (terminalFilter_)
        terminalFilter_->prepareForImageCapture();
}

void GPUImageFilterGroup::setTargetToIgnoreForUpdates(GPUImageInput* target) {
    if (terminalFilter_)
        terminalFilter_->setTargetToIgnoreForUpdates(target);
}

void GPUImageFilterGroup::addTarget(GPUImageInput* newTarget, gpu_int textureLocation) {
    if (terminalFilter_)
        terminalFilter_->addTarget(newTarget, textureLocation);
}

void GPUImageFilterGroup::removeTarget(GPUImageInput* targetToRemove) {
    if (terminalFilter_)
        terminalFilter_->removeTarget(targetToRemove);
}

void GPUImageFilterGroup::removeAllTargets() {
    if (terminalFilter_)
        terminalFilter_->removeAllTargets();
}
/*
- (void)setFrameProcessingCompletionBlock:(void (^)(GPUImageOutput *, CMTime))frameProcessingCompletionBlock;
{
    [_terminalFilter setFrameProcessingCompletionBlock:frameProcessingCompletionBlock];
}

- (void (^)(GPUImageOutput *, CMTime))frameProcessingCompletionBlock;
{
    return [_terminalFilter frameProcessingCompletionBlock];
}

#pragma mark -
#pragma mark GPUImageInput protocol
*/
void GPUImageFilterGroup::newFrameReadyAtTime(gpu_time frameTime, gpu_int textureIndex) {

    size_t numInitialFilters = initialFilters_.size();
    for (size_t k = 0; k < numInitialFilters; k++)
    {
        GPUImageInput* currentFilter = initialFilters_[k];
        if (currentFilter != inputFilterToIgnoreForUpdates_) {

            currentFilter->newFrameReadyAtTime(frameTime, textureIndex);
        }
    }
}

void GPUImageFilterGroup::setInputTexture(gpu_uint newInputTexture, gpu_int textureIndex) {

    size_t numInitialFilters = initialFilters_.size();
    for (size_t k = 0; k < numInitialFilters; k++) {

        GPUImageInput* currentFilter = initialFilters_[k];
        currentFilter->setInputTexture(newInputTexture, textureIndex);
    }
}

gpu_int GPUImageFilterGroup::nextAvailableTextureIndex() {
    return 0;
}

void GPUImageFilterGroup::setInputSize(gpu_float_size newSize, gpu_int textureIndex) {

    size_t numInitialFilters = initialFilters_.size();
    for (size_t k = 0; k < numInitialFilters; k++) {

        GPUImageInput* currentFilter = initialFilters_[k];
        currentFilter->setInputSize(newSize, textureIndex);
    }
}

void GPUImageFilterGroup::setInputRotation(GPUImageRotationMode newInputRotation, gpu_int textureIndex) {

    size_t numInitialFilters = initialFilters_.size();
    for (size_t k = 0; k < numInitialFilters; k++) {

        GPUImageInput* currentFilter = initialFilters_[k];
        currentFilter->setInputRotation(newInputRotation, textureIndex);
    }
}
/*
void GPUImageFilterGroup::forceProcessingAtSize:(CGSize)frameSize;
{
    size_t numInitialFilters = initialFilters_.size();
    for (size_t k = 0; k < numInitialFilters; k++) {

        GPUImageOutput* currentFilter = initialFilters_[k];

        currentFilter->forceProcessingAtSize(frameSize);
    }
}*/

gpu_float_size GPUImageFilterGroup::maximumOutputSize() {
    // I'm temporarily disabling adjustments for smaller output sizes until I figure out how to make this work better
    gpu_float_size zeroSize = {0.0f, 0.0f};

    return zeroSize;
}

void GPUImageFilterGroup::endProcessing()  {

    size_t numInitialFilters = initialFilters_.size();
    for (size_t k = 0; k < numInitialFilters; k++) {

        GPUImageInput* currentFilter = initialFilters_[k];

        currentFilter->endProcessing();
    }
}


bool GPUImageFilterGroup::shouldIgnoreUpdatesToThisTarget() {
    return shouldIgnoreUpdatesToThisTarget_;
}

void GPUImageFilterGroup::setShouldIgnoreUpdatesToThisTarget(bool ignore) {
    shouldIgnoreUpdatesToThisTarget_ = ignore;
}

bool GPUImageFilterGroup::enabled() {
    return enabled_;
}

void GPUImageFilterGroup::setEnabled(bool enable) {
    enabled_ = enable;
}