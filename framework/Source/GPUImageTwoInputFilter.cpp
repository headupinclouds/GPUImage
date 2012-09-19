/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */
 
#include "GPUImageTwoInputFilter.h"
#include "GPUImageOpenGLESContext.h"
#include "GLProgram.h"

const std::string GPUImageTwoInputFilter::kGPUImageTwoInputTextureVertexShaderString("\
 attribute vec4 position;\
 attribute vec4 inputTextureCoordinate;\
 attribute vec4 inputTextureCoordinate2;\
 \
 varying vec2 textureCoordinate;\
 varying vec2 textureCoordinate2;\
 \
 void main()\
 {\
     gl_Position = position;\
     textureCoordinate = inputTextureCoordinate.xy;\
     textureCoordinate2 = inputTextureCoordinate2.xy;\
 }"
);

GPUImageTwoInputFilter::GPUImageTwoInputFilter() :
    GPUImageFilter(),
    filterSecondTextureCoordinateAttribute_(0),
    filterInputTextureUniform2_(0),
    inputRotation2_(kGPUImageNoRotation),
    filterSourceTexture2_(0),
    //TODO: firstFrameTime_(0),
    //TODO: secondFrameTime_(0),
    hasSetFirstTexture_(false),
    hasReceivedFirstFrame_(false), 
    hasReceivedSecondFrame_(false),
    firstFrameWasVideo_(false),
    secondFrameWasVideo_(false) {

}

GPUImageTwoInputFilter::~GPUImageTwoInputFilter() {

}

void GPUImageTwoInputFilter::initWithFragmentShaderFromString(const std::string& fragmentShaderString) {

    initWithVertexShaderFromString(kGPUImageTwoInputTextureVertexShaderString, fragmentShaderString);
}

void GPUImageTwoInputFilter::initWithVertexShaderFromString(const std::string& vertexShaderString, const std::string& fragmentShaderString) {

    GPUImageFilter::initWithVertexShaderFromString(vertexShaderString, fragmentShaderString);
        
    inputRotation2_ = kGPUImageNoRotation;
    
    hasSetFirstTexture_ = false;
    
    hasReceivedFirstFrame_ = false;
    hasReceivedSecondFrame_ = false;
    firstFrameWasVideo_ = false;
    secondFrameWasVideo_ = false;
    
    //TODO: firstFrameTime_ = kCMTimeInvalid;
    //TODO: secondFrameTime_ = kCMTimeInvalid;
        
    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
        [GPUImageOpenGLESContext useImageProcessingContext];
        filterSecondTextureCoordinateAttribute = [filterProgram attributeIndex:@"inputTextureCoordinate2"];
        
        filterInputTextureUniform2 = [filterProgram uniformIndex:@"inputImageTexture2"]; // This does assume a name of "inputImageTexture2" for second input texture in the fragment shader
        glEnableVertexAttribArray(filterSecondTextureCoordinateAttribute);
    });*/
    GPUImageOpenGLESContext::useImageProcessingContext();
    filterSecondTextureCoordinateAttribute_ = filterProgram_->getAttributeIndex("inputTextureCoordinate2");

    // This does assume a name of "inputImageTexture2" for second input texture in the fragment shader
    filterInputTextureUniform2_ = filterProgram_->getUniformIndex("inputImageTexture2"); 
    glEnableVertexAttribArray(filterSecondTextureCoordinateAttribute_);
    // End runSynchronouslyOnVideoProcessingQueue block
}

void GPUImageTwoInputFilter::initializeAttributes() {

    GPUImageFilter::initializeAttributes();
    filterProgram_->addAttribute("inputTextureCoordinate2");
}

void GPUImageTwoInputFilter::renderToTextureWithVertices(const GLfloat* vertices, const GLfloat* textureCoordinates, GLuint sourceTexture) {
    
    if (preventRendering_) {
        return;
    }
    
    GPUImageOpenGLESContext::setActiveShaderProgram(filterProgram_);
    setUniformsForProgramAtIndex(0);

    setFilterFBO();
    
    glClearColor(backgroundColorRed_, backgroundColorGreen_, backgroundColorBlue_, backgroundColorAlpha_);
    glClear(GL_COLOR_BUFFER_BIT);
    
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, sourceTexture);
	glUniform1i(filterInputTextureUniform_, 2);	
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, filterSourceTexture2_);                
    glUniform1i(filterInputTextureUniform2_, 3);
    
    glVertexAttribPointer(filterPositionAttribute_, 2, GL_FLOAT, 0, 0, vertices);
	glVertexAttribPointer(filterTextureCoordinateAttribute_, 2, GL_FLOAT, 0, 0, textureCoordinates);
    glVertexAttribPointer(filterSecondTextureCoordinateAttribute_, 2, GL_FLOAT, 0, 0, textureCoordinatesForRotation(inputRotation2_));
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);    
}

gpu_int GPUImageTwoInputFilter::nextAvailableTextureIndex() {

    if (hasSetFirstTexture_) {
        return 1;
    }
    else
    {
        return 0;
    }
}

void GPUImageTwoInputFilter::setInputTexture(GLuint newInputTexture, gpu_int textureIndex) {

    if (textureIndex == 0) {
        filterSourceTexture_ = newInputTexture;
        hasSetFirstTexture_ = true;
    }
    else
    {
        filterSourceTexture2_ = newInputTexture;
    }
}

void GPUImageTwoInputFilter::setInputSize(gpu_float_size newSize, gpu_int textureIndex) {

    if (textureIndex == 0) {

        GPUImageFilter::setInputSize(newSize, textureIndex);
        
        if (FloatSizeEqualToZero(newSize)) {
            hasSetFirstTexture_ = false;
        }
    }
}

void GPUImageTwoInputFilter::setInputRotation(GPUImageRotationMode newInputRotation, gpu_int textureIndex) {

    if (textureIndex == 0) {
        inputRotation_ = newInputRotation;
    }
    else
    {
        inputRotation2_ = newInputRotation;
    }
}

gpu_float_size GPUImageTwoInputFilter::rotatedSize(gpu_float_size sizeToRotate, gpu_int textureIndex) {

    gpu_float_size rotatedSize = sizeToRotate;
    
    GPUImageRotationMode rotationToCheck;
    if (textureIndex == 0) {
        rotationToCheck = inputRotation_;
    }
    else
    {
        rotationToCheck = inputRotation2_;
    }
    
    if (GPUImageRotationSwapsWidthAndHeight(rotationToCheck))
    {
        rotatedSize.width = sizeToRotate.height;
        rotatedSize.height = sizeToRotate.width;
    }
    
    return rotatedSize; 
}

void GPUImageTwoInputFilter::newFrameReadyAtTime(gpu_time frameTime, gpu_int textureIndex) {

    // You can set up infinite update loops, so this helps to short circuit them
    if (hasReceivedFirstFrame_ && hasReceivedSecondFrame_) {
        return;
    }
    
    bool updatedMovieFrameOppositeStillImage = false;

    // TODO: try to understand what's this if/else for. For now, set updatedMovieFrameOppositeStillImage = true;
    if (textureIndex == 0) {
        hasReceivedFirstFrame_ = true;

        updatedMovieFrameOppositeStillImage = true; // TODO: remove me

        /*firstFrameTime = frameTime;
        
        if (!CMTIME_IS_INDEFINITE(frameTime))
        {
            if CMTIME_IS_INDEFINITE(secondFrameTime)
            {
                updatedMovieFrameOppositeStillImage = true;
            }
        }*/
    }
    else
    {
        hasReceivedSecondFrame_ = true;

        updatedMovieFrameOppositeStillImage = true; // TODO: remove me
        
        /*
        secondFrameTime = frameTime;

        if (!CMTIME_IS_INDEFINITE(frameTime))
        {
            if CMTIME_IS_INDEFINITE(firstFrameTime)
            {
                updatedMovieFrameOppositeStillImage = true;
            }
        }*/
    }
    
    if ((hasReceivedFirstFrame_ && hasReceivedSecondFrame_) || updatedMovieFrameOppositeStillImage)
    {
        GPUImageFilter::newFrameReadyAtTime(frameTime, 0);
        hasReceivedFirstFrame_ = false;
        hasReceivedSecondFrame_ = false;
    }
}
