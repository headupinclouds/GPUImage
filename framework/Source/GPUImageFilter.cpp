/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImageFilter.h"
#include "GPUImageOpenGLESContext.h"
#include "GLProgram.h"
#include "FreeImage.h"  // TODO: remove, just for debug purposes

// Hardcode the vertex shader for standard filters, but this can be overridden
const std::string GPUImageFilter::kGPUImageVertexShaderString("\
    attribute vec4 position;\
    attribute vec4 inputTextureCoordinate;\
\
    varying vec2 textureCoordinate;\
\
    void main()\
    {\
        gl_Position = position;\
        textureCoordinate = inputTextureCoordinate.xy;\
    }"
);

const std::string GPUImageFilter::kGPUImagePassthroughFragmentShaderString("\
    varying highp vec2 textureCoordinate;\
    \
    uniform sampler2D inputImageTexture;\
    \
    void main()\
    {\
        gl_FragColor = texture2D(inputImageTexture, textureCoordinate);\
    }"
);

GPUImageFilter::GPUImageFilter() : 
    filterSourceTexture_(0), 
    filterFramebuffer_(0), 
    filterProgram_(NULL), 
    filterPositionAttribute_(0),
    filterTextureCoordinateAttribute_(0), 
    filterInputTextureUniform_(0), 
    backgroundColorRed_(0.0f), 
    backgroundColorGreen_(0.0f), 
    backgroundColorBlue_(0.0f), 
    backgroundColorAlpha_(0.0f), 
    preparedToCaptureImage_(false), 
    inputRotation_(kGPUImageNoRotation), 
    preventRendering_(false),
    shouldIgnoreUpdatesToThisTarget_(false), 
    enabled_(false) {

    currentFilterSize_.width = 0.0f;
    currentFilterSize_.height = 0.0f;

}

GPUImageFilter::~GPUImageFilter() {
    destroyFilterFBO();
}

void GPUImageFilter::initWithVertexShaderFromString(const std::string& vertexShaderString, const std::string& fragmentShaderString) {

    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
    [GPUImageOpenGLESContext useImageProcessingContext];

    filterProgram = [[GPUImageOpenGLESContext sharedImageProcessingOpenGLESContext] programForVertexShaderString:vertexShaderString fragmentShaderString:fragmentShaderString];

    if (!filterProgram.initialized)
    {
    [self initializeAttributes];

    if (![filterProgram link])
    {
    NSString *progLog = [filterProgram programLog];
    NSLog(@"Program link log: %@", progLog);
    NSString *fragLog = [filterProgram fragmentShaderLog];
    NSLog(@"Fragment shader compile log: %@", fragLog);
    NSString *vertLog = [filterProgram vertexShaderLog];
    NSLog(@"Vertex shader compile log: %@", vertLog);
    filterProgram = nil;
    NSAssert(NO, @"Filter shader link failed");
    }
    }

    filterPositionAttribute = [filterProgram attributeIndex:@"position"];
    filterTextureCoordinateAttribute = [filterProgram attributeIndex:@"inputTextureCoordinate"];
    filterInputTextureUniform = [filterProgram uniformIndex:@"inputImageTexture"]; // This does assume a name of "inputImageTexture" for the fragment shader

    [GPUImageOpenGLESContext setActiveShaderProgram:filterProgram];

    glEnableVertexAttribArray(filterPositionAttribute);
    glEnableVertexAttribArray(filterTextureCoordinateAttribute);    
    });*/

    GPUImageOpenGLESContext::useImageProcessingContext();

    filterProgram_ = GPUImageOpenGLESContext::sharedImageProcessingOpenGLESContext().programForVertexShaderString(vertexShaderString, fragmentShaderString);

    if (!filterProgram_->isInitialized()) {

        initializeAttributes();

        if (!filterProgram_->link()) {

            std::string progLog;
            filterProgram_->getProgramLog(progLog);
            //TODO: NSLog(@"Program link log: %@", progLog);

            std::string fragLog;
            filterProgram_->getFragmentShaderLog(fragLog);
            //TODO: NSLog(@"Fragment shader compile log: %@", fragLog);

            std::string vertLog;
            filterProgram_->getVertexShaderLog(vertLog);
            //TODO: NSLog(@"Vertex shader compile log: %@", vertLog);
            
            filterProgram_ = NULL;
            //TODO: NSAssert(NO, @"Filter shader link failed");
            return;
        }
    }

    filterPositionAttribute_ = filterProgram_->getAttributeIndex("position");
    filterTextureCoordinateAttribute_ = filterProgram_->getAttributeIndex("inputTextureCoordinate");
    filterInputTextureUniform_ = filterProgram_->getUniformIndex("inputImageTexture"); // This does assume a name of "inputImageTexture" for the fragment shader

    GPUImageOpenGLESContext::setActiveShaderProgram(filterProgram_);

    glEnableVertexAttribArray(filterPositionAttribute_);
    glEnableVertexAttribArray(filterTextureCoordinateAttribute_);    
    // End runSynchronouslyOnVideoProcessingQueue block

    setEnabled(true);
}

void GPUImageFilter::initWithFragmentShaderFromString(const std::string& fragmentShaderString) {

    initWithVertexShaderFromString(kGPUImageVertexShaderString, fragmentShaderString);
}

void GPUImageFilter::initWithFragmentShaderFromFile(const std::string& fragmentShaderFilename) {

    // TODO: load strings from file

    //std::string fragmentShaderPathname = [[NSBundle mainBundle] pathForResource:fragmentShaderFilename ofType:@"fsh"];
    //std::string fragmentShaderString = [NSString stringWithContentsOfFile:fragmentShaderPathname encoding:NSUTF8StringEncoding error:nil];

    //initWithFragmentShaderFromString(fragmentShaderString);
}

void GPUImageFilter::initializeAttributes() {

    filterProgram_->addAttribute("position");
    filterProgram_->addAttribute("inputTextureCoordinate");

    // Override this, calling back to this super method, in order to add new attributes to your vertex shader
}

void GPUImageFilter::setupFilterForSize(gpu_float_size filterFrameSize) {
    // This is where you can override to provide some custom setup, if your filter has a size-dependent element
}

gpu_float_size GPUImageFilter::rotatedSize(gpu_float_size sizeToRotate, gpu_int textureIndex) {

    gpu_float_size rotatedSize = sizeToRotate;

    if (GPUImageRotationSwapsWidthAndHeight(inputRotation_)) {
        rotatedSize.width = sizeToRotate.height;
        rotatedSize.height = sizeToRotate.width;
    }

    return rotatedSize; 
}

gpu_float_point GPUImageFilter::rotatedPoint(gpu_float_point pointToRotate, GPUImageRotationMode rotation) {

    gpu_float_point rotatedPoint;

    switch(rotation)
    {
    case kGPUImageNoRotation: 
        return pointToRotate; 
        break;
    case kGPUImageFlipHorizonal:
    {
        rotatedPoint.x = 1.0f - pointToRotate.x;
        rotatedPoint.y = pointToRotate.y;
    }; break;
    case kGPUImageFlipVertical:
    {
        rotatedPoint.x = pointToRotate.x;
        rotatedPoint.y = 1.0f - pointToRotate.y;
    }; break;
    case kGPUImageRotateLeft:
    {
        rotatedPoint.x = 1.0f - pointToRotate.y;
        rotatedPoint.y = pointToRotate.x;
    }; break;
    case kGPUImageRotateRight:
    {
        rotatedPoint.x = pointToRotate.y;
        rotatedPoint.y = 1.0f - pointToRotate.x;
    }; break;
    case kGPUImageRotateRightFlipVertical:
    {
        rotatedPoint.x = pointToRotate.y;
        rotatedPoint.y = pointToRotate.x;
    }; break;
    case kGPUImageRotate180:
    {
        rotatedPoint.x = 1.0f - pointToRotate.x;
        rotatedPoint.y = 1.0f - pointToRotate.y;
    }; break;

    }

    return rotatedPoint;
}

void GPUImageFilter::recreateFilterFBO() {

    cachedMaximumOutputSize_.height = 0.0f;
    cachedMaximumOutputSize_.width = 0.0f;

    destroyFilterFBO();

    //if (!(GPUImageOpenGLESContext::supportsFastTextureUpload && preparedToCaptureImage)) {  
    if (preparedToCaptureImage_) {
        deleteOutputTexture();
        initializeOutputTexture();
    }
    
    setFilterFBO();
}

gpu_float_size GPUImageFilter::sizeOfFBO() {

    gpu_float_size outputSize = maximumOutputSize();
    if(FloatSizeEqualToZero(outputSize) || (inputTextureSize_.width < outputSize.width))
    {
        return inputTextureSize_;
    }
    else
    {
        return outputSize;
    }
}

void GPUImageFilter::createFilterFBOofSize(gpu_float_size currentFBOSize) {

    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
    [GPUImageOpenGLESContext useImageProcessingContext];
    glActiveTexture(GL_TEXTURE1);
    glGenFramebuffers(1, &filterFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, filterFramebuffer);

    if ([GPUImageOpenGLESContext supportsFastTextureUpload] && preparedToCaptureImage)
    {

    #if defined(__IPHONE_6_0)
    CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, [[GPUImageOpenGLESContext sharedImageProcessingOpenGLESContext] context], NULL, &filterTextureCache);
    #else
    CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, (__bridge void *)[[GPUImageOpenGLESContext sharedImageProcessingOpenGLESContext] context], NULL, &filterTextureCache);
    #endif

    if (err)
    {
    NSAssert(NO, @"Error at CVOpenGLESTextureCacheCreate %d", err);
    }

    // Code originally sourced from http://allmybrain.com/2011/12/08/rendering-to-a-texture-with-ios-5-texture-cache-api/

    CFDictionaryRef empty; // empty value for attr value.
    CFMutableDictionaryRef attrs;
    empty = CFDictionaryCreate(kCFAllocatorDefault, NULL, NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks); // our empty IOSurface properties dictionary
    attrs = CFDictionaryCreateMutable(kCFAllocatorDefault, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFDictionarySetValue(attrs, kCVPixelBufferIOSurfacePropertiesKey, empty);

    err = CVPixelBufferCreate(kCFAllocatorDefault, (int)currentFBOSize.width, (int)currentFBOSize.height, kCVPixelFormatType_32BGRA, attrs, &renderTarget);
    if (err)
    {
    NSLog(@"FBO size: %f, %f", currentFBOSize.width, currentFBOSize.height);
    NSAssert(NO, @"Error at CVPixelBufferCreate %d", err);
    }

    err = CVOpenGLESTextureCacheCreateTextureFromImage (kCFAllocatorDefault,
                                filterTextureCache, renderTarget,
                                NULL, // texture attributes
                                GL_TEXTURE_2D,
                                GL_RGBA, // opengl format
                                (int)currentFBOSize.width,
                                (int)currentFBOSize.height,
                                GL_BGRA, // native iOS format
                                GL_UNSIGNED_BYTE,
                                0,
                                &renderTexture);
    if (err)
    {
    NSAssert(NO, @"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
    }

    CFRelease(attrs);
    CFRelease(empty);
    glBindTexture(CVOpenGLESTextureGetTarget(renderTexture), CVOpenGLESTextureGetName(renderTexture));
    outputTexture = CVOpenGLESTextureGetName(renderTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CVOpenGLESTextureGetName(renderTexture), 0);

    [self notifyTargetsAboutNewOutputTexture];
    }
    else
    {
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)currentFBOSize.width, (int)currentFBOSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);

    [self notifyTargetsAboutNewOutputTexture];
    }

    //    NSLog(@"Filter size: %f, %f for filter: %@", currentFBOSize.width, currentFBOSize.height, self);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    NSAssert(status == GL_FRAMEBUFFER_COMPLETE, @"Incomplete filter FBO: %d", status);
    glBindTexture(GL_TEXTURE_2D, 0);
    });*/
    GPUImageOpenGLESContext::useImageProcessingContext();

    glActiveTexture(GL_TEXTURE1);
    glGenFramebuffers(1, &filterFramebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, filterFramebuffer_);

    // TODO: I don't know if supportsFastTextureUpload is iOS specific so I disabled this code block. (
    /*if ([GPUImageOpenGLESContext supportsFastTextureUpload] && preparedToCaptureImage)
        {
        #if defined(__IPHONE_6_0)
        CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, [[GPUImageOpenGLESContext sharedImageProcessingOpenGLESContext] context], NULL, &filterTextureCache);
        #else
        CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, (__bridge void *)[[GPUImageOpenGLESContext sharedImageProcessingOpenGLESContext] context], NULL, &filterTextureCache);
        #endif

        if (err)
        {
        NSAssert(NO, @"Error at CVOpenGLESTextureCacheCreate %d", err);
        }

        // Code originally sourced from http://allmybrain.com/2011/12/08/rendering-to-a-texture-with-ios-5-texture-cache-api/

        CFDictionaryRef empty; // empty value for attr value.
        CFMutableDictionaryRef attrs;
        empty = CFDictionaryCreate(kCFAllocatorDefault, NULL, NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks); // our empty IOSurface properties dictionary
        attrs = CFDictionaryCreateMutable(kCFAllocatorDefault, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        CFDictionarySetValue(attrs, kCVPixelBufferIOSurfacePropertiesKey, empty);

        err = CVPixelBufferCreate(kCFAllocatorDefault, (int)currentFBOSize.width, (int)currentFBOSize.height, kCVPixelFormatType_32BGRA, attrs, &renderTarget);
        if (err)
        {
        NSLog(@"FBO size: %f, %f", currentFBOSize.width, currentFBOSize.height);
        NSAssert(NO, @"Error at CVPixelBufferCreate %d", err);
        }

        err = CVOpenGLESTextureCacheCreateTextureFromImage (kCFAllocatorDefault,
                                    filterTextureCache, renderTarget,
                                    NULL, // texture attributes
                                    GL_TEXTURE_2D,
                                    GL_RGBA, // opengl format
                                    (int)currentFBOSize.width,
                                    (int)currentFBOSize.height,
                                    GL_BGRA, // native iOS format
                                    GL_UNSIGNED_BYTE,
                                    0,
                                    &renderTexture);
        if (err)
        {
        NSAssert(NO, @"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
        }

        CFRelease(attrs);
        CFRelease(empty);
        glBindTexture(CVOpenGLESTextureGetTarget(renderTexture), CVOpenGLESTextureGetName(renderTexture));
        outputTexture = CVOpenGLESTextureGetName(renderTexture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CVOpenGLESTextureGetName(renderTexture), 0);

        [self notifyTargetsAboutNewOutputTexture];
    }
    else*/
    {
        glBindTexture(GL_TEXTURE_2D, outputTexture_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)currentFBOSize.width, (int)currentFBOSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture_, 0);

        notifyTargetsAboutNewOutputTexture();
    }

    //    NSLog(@"Filter size: %f, %f for filter: %@", currentFBOSize.width, currentFBOSize.height, self);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    //TODO: NSAssert(status == GL_FRAMEBUFFER_COMPLETE, @"Incomplete filter FBO: %d", status);
    glBindTexture(GL_TEXTURE_2D, 0);
    // End runSynchronouslyOnVideoProcessingQueue block
}

void GPUImageFilter::destroyFilterFBO() {

    if (filterFramebuffer_)
    {
        // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
        // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
        /*runSynchronouslyOnVideoProcessingQueue(^{
        [GPUImageOpenGLESContext useImageProcessingContext];

        glDeleteFramebuffers(1, &filterFramebuffer);
        filterFramebuffer = 0;

        if (filterTextureCache != NULL)
        {
        CFRelease(renderTarget);
        renderTarget = NULL;

        if (renderTexture)
        {
        CFRelease(renderTexture);
        renderTexture = NULL;
        }

        CVOpenGLESTextureCacheFlush(filterTextureCache, 0);
        CFRelease(filterTextureCache);
        filterTextureCache = NULL;
        }
        });*/
        GPUImageOpenGLESContext::useImageProcessingContext();

        glDeleteFramebuffers(1, &filterFramebuffer_);
        filterFramebuffer_ = 0;

        // TODO: what's this block for?
        /*if (filterTextureCache_ != NULL) {
            CFRelease(renderTarget_);
            renderTarget = NULL;

            if (renderTexture_)
            {
                CFRelease(renderTexture_);
                renderTexture_ = NULL;
            }

            CVOpenGLESTextureCacheFlush(filterTextureCache, 0);
            CFRelease(filterTextureCache);
            filterTextureCache_ = NULL;
        }*/
        // End runSynchronouslyOnVideoProcessingQueue block
    }
}

void GPUImageFilter::setFilterFBO() {

    if (!filterFramebuffer_)
    {
        gpu_float_size currentFBOSize = sizeOfFBO();
        createFilterFBOofSize(currentFBOSize);
        setupFilterForSize(currentFBOSize);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, filterFramebuffer_);

    gpu_float_size currentFBOSize = sizeOfFBO();
    glViewport(0, 0, (int)currentFBOSize.width, (int)currentFBOSize.height);
}

void GPUImageFilter::setOutputFBO() {
    // Override this for filters that have multiple framebuffers
    setFilterFBO();
}

const GLfloat* GPUImageFilter::textureCoordinatesForRotation(GPUImageRotationMode rotationMode) {

    static const GLfloat noRotationTextureCoordinates[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };

    static const GLfloat rotateLeftTextureCoordinates[] = {
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
    };

    static const GLfloat rotateRightTextureCoordinates[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };

    static const GLfloat verticalFlipTextureCoordinates[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,
    };

    static const GLfloat horizontalFlipTextureCoordinates[] = {
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
    };

    static const GLfloat rotateRightVerticalFlipTextureCoordinates[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };

    static const GLfloat rotate180TextureCoordinates[] = {
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
    };

    switch(rotationMode)
    {
        case kGPUImageNoRotation: return noRotationTextureCoordinates;
        case kGPUImageRotateLeft: return rotateLeftTextureCoordinates;
        case kGPUImageRotateRight: return rotateRightTextureCoordinates;
        case kGPUImageFlipVertical: return verticalFlipTextureCoordinates;
        case kGPUImageFlipHorizonal: return horizontalFlipTextureCoordinates;
        case kGPUImageRotateRightFlipVertical: return rotateRightVerticalFlipTextureCoordinates;
        case kGPUImageRotate180: return rotate180TextureCoordinates;
    }

    // Added to avoid warning C4715
    return NULL;
}

void GPUImageFilter::renderToTextureWithVertices(const GLfloat* vertices, const GLfloat* textureCoordinates, GLuint sourceTexture) {

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

    glVertexAttribPointer(filterPositionAttribute_, 2, GL_FLOAT, 0, 0, vertices);
    glVertexAttribPointer(filterTextureCoordinateAttribute_, 2, GL_FLOAT, 0, 0, textureCoordinates);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // TODO: Remove this debug code
    gpu_float_size currentFBOSize = sizeOfFBO();
    size_t totalBytesForImage = (int)currentFBOSize.width * (int)currentFBOSize.height * 4;

    GLubyte *rawImagePixels2 = new GLubyte[totalBytesForImage];

    glReadPixels(0, 0, (int)currentFBOSize.width, (int)currentFBOSize.height, GL_RGBA, GL_UNSIGNED_BYTE, rawImagePixels2);
    FIBITMAP* bmp = FreeImage_ConvertFromRawBits(rawImagePixels2, (int)currentFBOSize.width, (int)currentFBOSize.height, 
        4 * (int)currentFBOSize.width, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);
    FreeImage_Save(FIF_PNG, bmp, "ImageFilterDump.png" , 0);
    
    delete [] rawImagePixels2;
    // end debug code

}

void GPUImageFilter::setUniformsForProgramAtIndex(gpu_uint programIndex) {

}

void GPUImageFilter::informTargetsAboutNewFrameAtTime(gpu_time frameTime) {
    /* TODO
    if (frameProcessingCompletionBlock_ != NULL)
    {
        frameProcessingCompletionBlock(self, frameTime);
    }*/

    size_t numberOfTargets = targets_.size();

    for (size_t k = 0; k < numberOfTargets; k++) {

        GPUImageInput* currentTarget = targets_[k];

        if(currentTarget != targetToIgnoreForUpdates_)
        {
            gpu_int textureIndex = targetTextureIndices_[k];

            /* TODO
            if ([GPUImageOpenGLESContext supportsFastTextureUpload] && preparedToCaptureImage)
            {
            [self setInputTextureForTarget:currentTarget atIndex:textureIndex];
            }*/

            currentTarget->setInputSize(getOutputFrameSize(), textureIndex);
            currentTarget->newFrameReadyAtTime(frameTime, textureIndex);
        }
    }
}

gpu_float_size GPUImageFilter::getOutputFrameSize() {
    return inputTextureSize_;
}

void GPUImageFilter::setBackgroundColor(GLfloat redComponent, GLfloat greenComponent, GLfloat blueComponent, GLfloat alphaComponent) {

    backgroundColorRed_ = redComponent;
    backgroundColorGreen_ = greenComponent;
    backgroundColorBlue_ = blueComponent;
    backgroundColorAlpha_ = alphaComponent;
}

void GPUImageFilter::setInteger(GLint newInteger, const std::string& uniformName) {
    GLint uniformIndex = filterProgram_->getUniformIndex(uniformName);
    setInteger(newInteger, uniformIndex, filterProgram_);
}

void GPUImageFilter::setFloat(GLfloat newFloat, const std::string& uniformName) {
    GLint uniformIndex = filterProgram_->getUniformIndex(uniformName);
    setFloat(newFloat, uniformIndex, filterProgram_);
}

void GPUImageFilter::setSize(gpu_float_size newSize, const std::string& uniformName) {
    GLint uniformIndex = filterProgram_->getUniformIndex(uniformName);
    setSize(newSize, uniformIndex, filterProgram_);
}

void GPUImageFilter::setPoint(gpu_float_point newPoint, const std::string& uniformName) {
    GLint uniformIndex = filterProgram_->getUniformIndex(uniformName);
    setPoint(newPoint, uniformIndex, filterProgram_);
}

void GPUImageFilter::setFloatVec3(GPUVector3 newVec3, const std::string& uniformName) {
    GLint uniformIndex = filterProgram_->getUniformIndex(uniformName);
    setVec3(newVec3, uniformIndex, filterProgram_);
}

void GPUImageFilter::setFloatVec4(GPUVector4 newVec4, const std::string& uniformName) {
    GLint uniformIndex = filterProgram_->getUniformIndex(uniformName);
    setVec4(newVec4, uniformIndex, filterProgram_);
}

void GPUImageFilter::setFloatArray(GLfloat* array, GLsizei count, const std::string& uniformName) {
    GLint uniformIndex = filterProgram_->getUniformIndex(uniformName);
    
    setFloatArray(array, count, uniformIndex, filterProgram_);
}

void GPUImageFilter::setMatrix3f(GPUMatrix3x3 matrix, GLint uniform, GLProgram* shaderProgram) {
    // TODO: run those command on the GLES processing thread and do not wait for the results
    // dispatch_async([GPUImageOpenGLESContext sharedOpenGLESQueue], ^{
    GPUImageOpenGLESContext::setActiveShaderProgram(shaderProgram);
    glUniformMatrix3fv(uniform, 1, GL_FALSE, (GLfloat *)&matrix);
    // End dispatch_async block
}

void GPUImageFilter::setMatrix4f(GPUMatrix4x4 matrix, GLint uniform, GLProgram* shaderProgram) {
    // TODO: run those command on the GLES processing thread and do not wait for the results
    // dispatch_async([GPUImageOpenGLESContext sharedOpenGLESQueue], ^{
    GPUImageOpenGLESContext::setActiveShaderProgram(shaderProgram);
    glUniformMatrix4fv(uniform, 1, GL_FALSE, (GLfloat *)&matrix);
    // End dispatch_async block
}

void GPUImageFilter::setFloat(GLfloat floatValue, GLint uniform, GLProgram* shaderProgram) {
    // TODO: run those command on the GLES processing thread and do not wait for the results
    // dispatch_async([GPUImageOpenGLESContext sharedOpenGLESQueue], ^{
    GPUImageOpenGLESContext::setActiveShaderProgram(shaderProgram);
    glUniform1f(uniform, floatValue);
    // End dispatch_async block
}

void GPUImageFilter::setPoint(gpu_float_point pointValue, GLint uniform, GLProgram* shaderProgram) {
    // TODO: run those command on the GLES processing thread and do not wait for the results
    // dispatch_async([GPUImageOpenGLESContext sharedOpenGLESQueue], ^{
    GPUImageOpenGLESContext::setActiveShaderProgram(shaderProgram);

    GLfloat positionArray[2];
    positionArray[0] = pointValue.x;
    positionArray[1] = pointValue.y;
    
    glUniform2fv(uniform, 1, positionArray);
    // End dispatch_async block
}

void GPUImageFilter::setSize(gpu_float_size sizeValue, GLint uniform, GLProgram* shaderProgram) {
    // TODO: run those command on the GLES processing thread and do not wait for the results
    // dispatch_async([GPUImageOpenGLESContext sharedOpenGLESQueue], ^{
    GPUImageOpenGLESContext::setActiveShaderProgram(shaderProgram);
    
    GLfloat sizeArray[2];
    sizeArray[0] = sizeValue.width;
    sizeArray[1] = sizeValue.height;
    
    glUniform2fv(uniform, 1, sizeArray);
    // End dispatch_async block
}

void GPUImageFilter::setVec3(GPUVector3 vectorValue, GLint uniform, GLProgram* shaderProgram) {
    // TODO: run those command on the GLES processing thread and do not wait for the results
    // dispatch_async([GPUImageOpenGLESContext sharedOpenGLESQueue], ^{
    GPUImageOpenGLESContext::setActiveShaderProgram(shaderProgram);

    glUniform3fv(uniform, 1, (GLfloat *)&vectorValue);
    // End dispatch_async block
}

void GPUImageFilter::setVec4(GPUVector4 vectorValue, GLint uniform, GLProgram* shaderProgram) {
    // TODO: run those command on the GLES processing thread and do not wait for the results
    // dispatch_async([GPUImageOpenGLESContext sharedOpenGLESQueue], ^{
    GPUImageOpenGLESContext::setActiveShaderProgram(shaderProgram);
    
    glUniform4fv(uniform, 1, (GLfloat *)&vectorValue);
    // End dispatch_async block
}

void GPUImageFilter::setFloatArray(GLfloat* arrayValue, GLsizei arrayLength, GLint uniform, GLProgram* shaderProgram) {
    // TODO: run those command on the GLES processing thread and do not wait for the results
    // dispatch_async([GPUImageOpenGLESContext sharedOpenGLESQueue], ^{
    GPUImageOpenGLESContext::setActiveShaderProgram(shaderProgram);
    
    glUniform1fv(uniform, arrayLength, arrayValue);
    // End dispatch_async block
}

void GPUImageFilter::setInteger(GLint intValue, GLint uniform, GLProgram* shaderProgram) {
    // TODO: run those command on the GLES processing thread and do not wait for the results
    // dispatch_async([GPUImageOpenGLESContext sharedOpenGLESQueue], ^{
    GPUImageOpenGLESContext::setActiveShaderProgram(shaderProgram);
    glUniform1i(uniform, intValue);
    // End dispatch_async block
}

void GPUImageFilter::newFrameReadyAtTime(gpu_time frameTime, gpu_int textureIndex) {
    
    static const GLfloat imageVertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f,  1.0f,
        1.0f,  1.0f,
    };

    renderToTextureWithVertices(imageVertices, textureCoordinatesForRotation(inputRotation_), filterSourceTexture_);

    informTargetsAboutNewFrameAtTime(frameTime);
}

void GPUImageFilter::setInputTexture(GLuint newInputTexture, gpu_int textureIndex) {
    filterSourceTexture_ = newInputTexture;
}

gpu_int GPUImageFilter::nextAvailableTextureIndex() {
    return 0;
}

void GPUImageFilter::setInputSize(gpu_float_size newSize, gpu_int textureIndex) {
    
    if (preventRendering_) {
        return;
    }

    if (overrideInputSize_) {

        if (FloatSizeEqualToZero(forcedMaximumSize_)) {
            return;
        }
        else
        {
            // TODO: what's this block for?
            /*CGRect insetRect = AVMakeRectWithAspectRatioInsideRect(newSize, CGRectMake(0.0, 0.0, forcedMaximumSize.width, forcedMaximumSize.height));
            inputTextureSize = insetRect.size;*/
            return;
        }
    }

    gpu_float_size rotSize = rotatedSize(newSize, textureIndex);

    if (FloatSizeEqualToZero(rotSize)) {
        inputTextureSize_ = rotSize;
    }
    else if (!FloatSizeEqualToSize(inputTextureSize_, rotSize))
    {
        inputTextureSize_ = rotSize;
        recreateFilterFBO();
    }
}

void GPUImageFilter::setInputRotation(GPUImageRotationMode newInputRotation, gpu_int textureIndex) {
    inputRotation_ = newInputRotation;
}

gpu_float_size GPUImageFilter::maximumOutputSize() {
    // I'm temporarily disabling adjustments for smaller output sizes until I figure out how to make this work better
    gpu_float_size zeroSize;
    zeroSize.height = zeroSize.width = 0.0f;

    return zeroSize;
}

void GPUImageFilter::endProcessing() {
    size_t numberOfTargets = targets_.size();

    for (size_t k = 0; k < numberOfTargets; k++) {

        GPUImageInput* currentTarget = targets_[k];
        currentTarget->endProcessing();
    }
}

bool GPUImageFilter::shouldIgnoreUpdatesToThisTarget() {
    return shouldIgnoreUpdatesToThisTarget_;
}

void GPUImageFilter::setShouldIgnoreUpdatesToThisTarget(bool ignore) {
    shouldIgnoreUpdatesToThisTarget_ = ignore;
}

bool GPUImageFilter::enabled() {
    return enabled_;
}

void GPUImageFilter::setEnabled(bool enable) {
    enabled_ = enable;
}