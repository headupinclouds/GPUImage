/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImageTwoPassFilter.h"
#include "GPUImageOpenGLESContext.h"
#include "GLProgram.h"

GPUImageTwoPassFilter::GPUImageTwoPassFilter() :
    GPUImageFilter(),
    secondFilterProgram_(NULL),
    secondFilterOutputTexture_(0),
    secondFilterPositionAttribute_(0),
    secondFilterTextureCoordinateAttribute_(0),
    secondFilterInputTextureUniform_(0), 
    secondFilterInputTextureUniform2_(0),
    secondFilterFramebuffer_(0) {

}

GPUImageTwoPassFilter::~GPUImageTwoPassFilter() {

}

void GPUImageTwoPassFilter::initWithFirstStageVertexShaderFromString(const std::string& firstStageVertexShaderString, const std::string& firstStageFragmentShaderString, 
        const std::string& secondStageVertexShaderString, const std::string& secondStageFragmentShaderString) {

    GPUImageFilter::initWithVertexShaderFromString(firstStageVertexShaderString, firstStageFragmentShaderString);

    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
        [GPUImageOpenGLESContext useImageProcessingContext];

        secondFilterProgram = [[GPUImageOpenGLESContext sharedImageProcessingOpenGLESContext] programForVertexShaderString:secondStageVertexShaderString fragmentShaderString:secondStageFragmentShaderString];
        
        if (!secondFilterProgram.initialized)
        {
            [self initializeSecondaryAttributes];
            
            if (![secondFilterProgram link])
            {
                NSString *progLog = [secondFilterProgram programLog];
                NSLog(@"Program link log: %@", progLog);
                NSString *fragLog = [secondFilterProgram fragmentShaderLog];
                NSLog(@"Fragment shader compile log: %@", fragLog);
                NSString *vertLog = [secondFilterProgram vertexShaderLog];
                NSLog(@"Vertex shader compile log: %@", vertLog);
                filterProgram = nil;
                NSAssert(NO, @"Filter shader link failed");
            }
        }
        
        secondFilterPositionAttribute = [secondFilterProgram attributeIndex:@"position"];
        secondFilterTextureCoordinateAttribute = [secondFilterProgram attributeIndex:@"inputTextureCoordinate"];
        secondFilterInputTextureUniform = [secondFilterProgram uniformIndex:@"inputImageTexture"]; // This does assume a name of "inputImageTexture" for the fragment shader
        secondFilterInputTextureUniform2 = [secondFilterProgram uniformIndex:@"inputImageTexture2"]; // This does assume a name of "inputImageTexture2" for second input texture in the fragment shader
        
        [GPUImageOpenGLESContext setActiveShaderProgram:secondFilterProgram];
        
        glEnableVertexAttribArray(secondFilterPositionAttribute);
        glEnableVertexAttribArray(secondFilterTextureCoordinateAttribute);
    });*/
    GPUImageOpenGLESContext::useImageProcessingContext();

    secondFilterProgram_ = GPUImageOpenGLESContext::sharedImageProcessingOpenGLESContext().programForVertexShaderString(
        secondStageVertexShaderString, secondStageFragmentShaderString);

    if (!secondFilterProgram_->isInitialized()) {

        initializeSecondaryAttributes();

        if (secondFilterProgram_->link()) {

            std::string progLog;
            secondFilterProgram_->getProgramLog(progLog);
            //TODO: NSLog(@"Program link log: %@", progLog);
            
            std::string fragLog;
            secondFilterProgram_->getFragmentShaderLog(fragLog);
            //TODO: NSLog(@"Fragment shader compile log: %@", fragLog);

            std::string vertLog;
            secondFilterProgram_->getVertexShaderLog(vertLog);
            //TODO: NSLog(@"Vertex shader compile log: %@", vertLog);
            
            filterProgram_ = NULL;
            //TODO: NSAssert(NO, @"Filter shader link failed");
        }
    }

    secondFilterPositionAttribute_ = secondFilterProgram_->getAttributeIndex("position");
    secondFilterTextureCoordinateAttribute_ = secondFilterProgram_->getAttributeIndex("inputTextureCoordinate");
    
    // This does assume a name of "inputImageTexture" for the fragment shader
    secondFilterInputTextureUniform_ = secondFilterProgram_->getUniformIndex("inputImageTexture"); 
    
    // This does assume a name of "inputImageTexture2" for second input texture in the fragment shader
    secondFilterInputTextureUniform2_ = secondFilterProgram_->getUniformIndex("inputImageTexture2"); 

    GPUImageOpenGLESContext::setActiveShaderProgram(secondFilterProgram_);

    glEnableVertexAttribArray(secondFilterPositionAttribute_);
    glEnableVertexAttribArray(secondFilterTextureCoordinateAttribute_);
    // End runSynchronouslyOnVideoProcessingQueue block
}

void GPUImageTwoPassFilter::initWithFirstStageFragmentShaderFromString(const std::string& firstStageFragmentShaderString, const std::string& secondStageFragmentShaderString) {
   
    initWithFirstStageVertexShaderFromString(kGPUImageVertexShaderString, firstStageFragmentShaderString, kGPUImageVertexShaderString, firstStageFragmentShaderString);
}

void GPUImageTwoPassFilter::initializeSecondaryAttributes() {

    secondFilterProgram_->addAttribute("position");
	secondFilterProgram_->addAttribute("inputTextureCoordinate");
}

GLuint GPUImageTwoPassFilter::textureForOutput() {
    return secondFilterOutputTexture_;
}

void GPUImageTwoPassFilter::initializeOutputTexture() {

    GPUImageFilter::initializeOutputTexture();

    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
        [GPUImageOpenGLESContext useImageProcessingContext];

        glGenTextures(1, &secondFilterOutputTexture);
        glBindTexture(GL_TEXTURE_2D, secondFilterOutputTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
    });*/
    GPUImageOpenGLESContext::useImageProcessingContext();

    glGenTextures(1, &secondFilterOutputTexture_);
    glBindTexture(GL_TEXTURE_2D, secondFilterOutputTexture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    // End runSynchronouslyOnVideoProcessingQueue block
}

void GPUImageTwoPassFilter::deleteOutputTexture() {

    GPUImageFilter::deleteOutputTexture();
    
    if (secondFilterOutputTexture_) {

        glDeleteTextures(1, &secondFilterOutputTexture_);
        secondFilterOutputTexture_ = 0;
    }
}

void GPUImageTwoPassFilter::createFilterFBOofSize(gpu_float_size currentFBOSize) {

    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
        [GPUImageOpenGLESContext useImageProcessingContext];

        if ([GPUImageOpenGLESContext supportsFastTextureUpload] && preparedToCaptureImage)
        {
            preparedToCaptureImage = NO;
            [super createFilterFBOofSize:currentFBOSize];
            preparedToCaptureImage = YES;
        }
        else
        {
            [super createFilterFBOofSize:currentFBOSize];
        }
        
        glGenFramebuffers(1, &secondFilterFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, secondFilterFramebuffer);

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
            secondFilterOutputTexture = CVOpenGLESTextureGetName(renderTexture);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CVOpenGLESTextureGetName(renderTexture), 0);
            
            [self notifyTargetsAboutNewOutputTexture];
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, secondFilterOutputTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)currentFBOSize.width, (int)currentFBOSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, secondFilterOutputTexture, 0);
            
            [self notifyTargetsAboutNewOutputTexture];
        }
        
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        
        NSAssert(status == GL_FRAMEBUFFER_COMPLETE, @"Incomplete filter FBO: %d", status);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    });*/
    //TODO
    // End runSynchronouslyOnVideoProcessingQueue block
}

void GPUImageTwoPassFilter::destroyFilterFBO() {

    GPUImageFilter::destroyFilterFBO();
    
    if (secondFilterFramebuffer_) 	{
		glDeleteFramebuffers(1, &secondFilterFramebuffer_);
		secondFilterFramebuffer_ = 0;
	}	
}

void GPUImageTwoPassFilter::setSecondFilterFBO() {

    glBindFramebuffer(GL_FRAMEBUFFER, secondFilterFramebuffer_);
}

void GPUImageTwoPassFilter::setOutputFBO() {
    setSecondFilterFBO();
}

void GPUImageTwoPassFilter::renderToTextureWithVertices(const GLfloat* vertices, const GLfloat* textureCoordinates, GLuint sourceTexture) {

    if (preventRendering_) {
        return;
    }
    
    // Run the first stage of the two-pass filter
    GPUImageFilter::renderToTextureWithVertices(vertices, textureCoordinates, sourceTexture);
    
    // Run the second stage of the two-pass filter
    setSecondFilterFBO();
    
    GPUImageOpenGLESContext::setActiveShaderProgram(secondFilterProgram_);
    setUniformsForProgramAtIndex(1);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, outputTexture_);
    
	glUniform1i(secondFilterInputTextureUniform_, 3);	
    
    glVertexAttribPointer(secondFilterPositionAttribute_, 2, GL_FLOAT, 0, 0, vertices);
	glVertexAttribPointer(secondFilterTextureCoordinateAttribute_, 2, GL_FLOAT, 0, 0, textureCoordinatesForRotation(kGPUImageNoRotation));
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void GPUImageTwoPassFilter::prepareForImageCapture() {
/*
    preparedToCaptureImage_ = true;
    
    if (GPUImageOpenGLESContext::supportsFastTextureUpload])
    {
        if (secondFilterOutputTexture)
        {
            runSynchronouslyOnVideoProcessingQueue(^{
                [GPUImageOpenGLESContext useImageProcessingContext];

                glDeleteTextures(1, &secondFilterOutputTexture);
                secondFilterOutputTexture = 0;
            });
        }
    }*/

    // TODO
}
