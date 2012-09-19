/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImageAverageColor.h"
#include "GLProgram.h"
#include "GPUImageOpenGLESContext.h"
#include <math.h>
#include <algorithm>

const std::string GPUImageAverageColor::kGPUImageColorAveragingVertexShaderString("\
 attribute vec4 position;\
 attribute vec4 inputTextureCoordinate;\
 \
 uniform highp float texelWidth;\
 uniform highp float texelHeight;\
 \
 varying vec2 upperLeftInputTextureCoordinate;\
 varying vec2 upperRightInputTextureCoordinate;\
 varying vec2 lowerLeftInputTextureCoordinate;\
 varying vec2 lowerRightInputTextureCoordinate;\
 \
 void main()\
 {\
     gl_Position = position;\
     \
     upperLeftInputTextureCoordinate = inputTextureCoordinate.xy + vec2(-texelWidth, -texelHeight);\
     upperRightInputTextureCoordinate = inputTextureCoordinate.xy + vec2(texelWidth, -texelHeight);\
     lowerLeftInputTextureCoordinate = inputTextureCoordinate.xy + vec2(-texelWidth, texelHeight);\
     lowerRightInputTextureCoordinate = inputTextureCoordinate.xy + vec2(texelWidth, texelHeight);\
 }"
);

const std::string GPUImageAverageColor::kGPUImageColorAveragingFragmentShaderString("\
 precision highp float;\
 \
 uniform sampler2D inputImageTexture;\
 \
 varying highp vec2 outputTextureCoordinate;\
 \
 varying highp vec2 upperLeftInputTextureCoordinate;\
 varying highp vec2 upperRightInputTextureCoordinate;\
 varying highp vec2 lowerLeftInputTextureCoordinate;\
 varying highp vec2 lowerRightInputTextureCoordinate;\
 \
 void main()\
 {\
     highp vec4 upperLeftColor = texture2D(inputImageTexture, upperLeftInputTextureCoordinate);\
     highp vec4 upperRightColor = texture2D(inputImageTexture, upperRightInputTextureCoordinate);\
     highp vec4 lowerLeftColor = texture2D(inputImageTexture, lowerLeftInputTextureCoordinate);\
     highp vec4 lowerRightColor = texture2D(inputImageTexture, lowerRightInputTextureCoordinate);\
     \
     gl_FragColor = 0.25 * (upperLeftColor + upperRightColor + lowerLeftColor + lowerRightColor);\
 }"
);

GPUImageAverageColor::GPUImageAverageColor() :
    GPUImageFilter() 
{

    GPUImageFilter::initWithVertexShaderFromString(kGPUImageColorAveragingVertexShaderString, kGPUImageColorAveragingFragmentShaderString);
    
    texelWidthUniform_ = filterProgram_->getUniformIndex("texelWidth");
    texelHeightUniform_ = filterProgram_->getUniformIndex("texelHeight");
    /*
    stageTextures_ = [[NSMutableArray alloc] init];
    stageFramebuffers_ = [[NSMutableArray alloc] init];
    stageSizes_ = [[NSMutableArray alloc] init];
    
    __unsafe_unretained GPUImageAverageColor *weakSelf = self;
    [self setFrameProcessingCompletionBlock:^(GPUImageOutput *filter, CMTime frameTime) {
        [weakSelf extractAverageColorAtFrameTime:frameTime];
    }];*/
}

void GPUImageAverageColor::initializeOutputTexture()
{
    if (inputTextureSize_.width < 1.0)
    {
        return;
    }
    
    // Create textures for each level
    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
        [GPUImageOpenGLESContext useImageProcessingContext];

        gpu_uint numberOfReductionsInX = floor(log(inputTextureSize.width) / log(4.0));
        gpu_uint numberOfReductionsInY = floor(log(inputTextureSize.height) / log(4.0));
//        NSLog(@"Reductions in X: %d, y: %d", numberOfReductionsInX, numberOfReductionsInY);
        
        gpu_uint reductionsToHitSideLimit = MIN(numberOfReductionsInX, numberOfReductionsInY);
//        NSLog(@"Total reductions: %d", reductionsToHitSideLimit);
        for (gpu_uint currentReduction = 0; currentReduction < reductionsToHitSideLimit; currentReduction++)
        {
//            gpu_float_size currentStageSize = gpu_float_sizeMake(ceil(inputTextureSize.width / pow(4.0, currentReduction + 1.0)), ceil(inputTextureSize.height / pow(4.0, currentReduction + 1.0)));
            gpu_float_size currentStageSize = gpu_float_sizeMake(floor(inputTextureSize.width / pow(4.0, currentReduction + 1.0)), floor(inputTextureSize.height / pow(4.0, currentReduction + 1.0)));
            if ( (currentStageSize.height < 2.0) || (currentStageSize.width < 2.0) )
            {
                // A really small last stage seems to cause significant errors in the average, so I abort and leave the rest to the CPU at this point
                break;
//                currentStageSize.height = 2.0; // TODO: Rotate the image to account for this case, which causes FBO construction to fail
            }
            
            [stageSizes addObject:[NSValue valueWithgpu_float_size:currentStageSize]];

            GLuint textureForStage;
            glGenTextures(1, &textureForStage);
            glBindTexture(GL_TEXTURE_2D, textureForStage);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            [stageTextures addObject:[NSNumber numberWithInt:textureForStage]];
            
//            NSLog(@"At reduction: %d size in X: %f, size in Y:%f", currentReduction, currentStageSize.width, currentStageSize.height);
        }
    });*/
    GPUImageOpenGLESContext::useImageProcessingContext();

    gpu_uint numberOfReductionsInX = static_cast<gpu_uint>(floor(log(inputTextureSize_.width) / log(4.0)));
    gpu_uint numberOfReductionsInY = static_cast<gpu_uint>(floor(log(inputTextureSize_.height) / log(4.0)));
    //        NSLog(@"Reductions in X: %d, y: %d", numberOfReductionsInX, numberOfReductionsInY);

    // Using (std::min) to avoid "min" macro expansion on MSVC
    gpu_uint reductionsToHitSideLimit = (std::min)(numberOfReductionsInX, numberOfReductionsInY);

    //        NSLog(@"Total reductions: %d", reductionsToHitSideLimit);
    for (gpu_uint currentReduction = 0; currentReduction < reductionsToHitSideLimit; currentReduction++)
    {
        //            gpu_float_size currentStageSize = gpu_float_sizeMake(ceil(inputTextureSize.width / pow(4.0, currentReduction + 1.0)), ceil(inputTextureSize.height / pow(4.0, currentReduction + 1.0)));
        gpu_float_size currentStageSize = {
            floor(inputTextureSize_.width / pow(4.0, currentReduction + 1.0)), 
            floor(inputTextureSize_.height / pow(4.0, currentReduction + 1.0))
        };

        if ( (currentStageSize.height < 2.0) || (currentStageSize.width < 2.0) )
        {
            // A really small last stage seems to cause significant errors in the average, so I abort and leave the rest to the CPU at this point
            break;
            //currentStageSize.height = 2.0; // TODO: Rotate the image to account for this case, which causes FBO construction to fail
        }

        stageSizes_.push_back(currentStageSize);

        GLuint textureForStage;
        glGenTextures(1, &textureForStage);
        glBindTexture(GL_TEXTURE_2D, textureForStage);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        stageTextures_.push_back(textureForStage);

        //            NSLog(@"At reduction: %d size in X: %f, size in Y:%f", currentReduction, currentStageSize.width, currentStageSize.height);
    }
    // End runSynchronouslyOnVideoProcessingQueue block
}

void GPUImageAverageColor::deleteOutputTexture()
{
    if (stageTextures_.size() == 0)
    {
        return;
    }
    
    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
        [GPUImageOpenGLESContext useImageProcessingContext];
        
        gpu_uint numberOfStageTextures = [stageTextures count];
        for (gpu_uint currentStage = 0; currentStage < numberOfStageTextures; currentStage++)
        {
            GLuint currentTexture = [[stageTextures objectAtIndex:currentStage] intValue];
            glDeleteTextures(1, &currentTexture);
        }
        
        [stageTextures removeAllObjects];
        [stageSizes removeAllObjects];
    });*/
    GPUImageOpenGLESContext::useImageProcessingContext();

    gpu_uint numberOfStageTextures = stageTextures_.size();
    for (gpu_uint currentStage = 0; currentStage < numberOfStageTextures; currentStage++)
    {
        GLuint currentTexture = stageTextures_[currentStage];
        glDeleteTextures(1, &currentTexture);
    }

    stageTextures_.clear();
    stageSizes_.clear();
    // End runSynchronouslyOnVideoProcessingQueue block
}

void GPUImageAverageColor::recreateFilterFBO()
{
    cachedMaximumOutputSize_.width = 0.0f;
    cachedMaximumOutputSize_.height = 0.0f;

    destroyFilterFBO();    
    deleteOutputTexture();
    initializeOutputTexture();
    
    setFilterFBO();
}

void GPUImageAverageColor::createFilterFBOofSize(gpu_float_size currentFBOSize)
{
    // Create framebuffers for each level

    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
        [GPUImageOpenGLESContext useImageProcessingContext];
        glActiveTexture(GL_TEXTURE1);
        
        gpu_uint numberOfStageFramebuffers = [stageTextures count];
        for (gpu_uint currentStage = 0; currentStage < numberOfStageFramebuffers; currentStage++)
        {
            GLuint currentFramebuffer;
            glGenFramebuffers(1, &currentFramebuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer);
            [stageFramebuffers addObject:[NSNumber numberWithInt:currentFramebuffer]];
            
            GLuint currentTexture = [[stageTextures objectAtIndex:currentStage] intValue];
            glBindTexture(GL_TEXTURE_2D, currentTexture);
            
            gpu_float_size currentFramebufferSize = [[stageSizes objectAtIndex:currentStage] gpu_float_sizeValue];
            
//            NSLog(@"FBO stage size: %f, %f", currentFramebufferSize.width, currentFramebufferSize.height);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)currentFramebufferSize.width, (int)currentFramebufferSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, currentTexture, 0);
            GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            
            NSAssert(status == GL_FRAMEBUFFER_COMPLETE, @"Incomplete filter FBO: %d", status);
        }
    });*/
    GPUImageOpenGLESContext::useImageProcessingContext();
    glActiveTexture(GL_TEXTURE1);

    gpu_uint numberOfStageFramebuffers = stageTextures_.size();
    for (gpu_uint currentStage = 0; currentStage < numberOfStageFramebuffers; currentStage++)
    {
        GLuint currentFramebuffer;
        glGenFramebuffers(1, &currentFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer);
        stageFramebuffers_.push_back(currentFramebuffer);

        GLuint currentTexture = stageTextures_[currentStage];
        glBindTexture(GL_TEXTURE_2D, currentTexture);

        gpu_float_size currentFramebufferSize = stageSizes_[currentStage];

        //            NSLog(@"FBO stage size: %f, %f", currentFramebufferSize.width, currentFramebufferSize.height);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)currentFramebufferSize.width, (int)currentFramebufferSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, currentTexture, 0);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        // TODO: NSAssert(status == GL_FRAMEBUFFER_COMPLETE, @"Incomplete filter FBO: %d", status);
    }
    // End runSynchronouslyOnVideoProcessingQueue block
}

void GPUImageAverageColor::destroyFilterFBO()
{
    // TODO: schedules that block job in the thread which owns the gles context and waits for its completition
    // TODO: Was run in a runSynchronouslyOnVideoProcessingQueue block
    /*runSynchronouslyOnVideoProcessingQueue(^{
        [GPUImageOpenGLESContext useImageProcessingContext];
        
        gpu_uint numberOfStageFramebuffers = [stageFramebuffers count];
        for (gpu_uint currentStage = 0; currentStage < numberOfStageFramebuffers; currentStage++)
        {
            GLuint currentFramebuffer = [[stageFramebuffers objectAtIndex:currentStage] intValue];
            glDeleteFramebuffers(1, &currentFramebuffer);
        }
        
        [stageFramebuffers removeAllObjects];
    });*/
    GPUImageOpenGLESContext::useImageProcessingContext();

    gpu_uint numberOfStageFramebuffers = stageFramebuffers_.size();
    for (gpu_uint currentStage = 0; currentStage < numberOfStageFramebuffers; currentStage++)
    {
        GLuint currentFramebuffer = stageFramebuffers_[currentStage];
        glDeleteFramebuffers(1, &currentFramebuffer);
    }

    stageFramebuffers_.clear();
    // End runSynchronouslyOnVideoProcessingQueue block
}

void GPUImageAverageColor::renderToTextureWithVertices(const GLfloat* vertices, const GLfloat* textureCoordinates, GLuint sourceTexture)
{
    if (preventRendering_)
    {
        return;
    }
    
    GPUImageOpenGLESContext::setActiveShaderProgram(filterProgram_);

    glVertexAttribPointer(filterPositionAttribute_, 2, GL_FLOAT, 0, 0, vertices);
    glVertexAttribPointer(filterTextureCoordinateAttribute_, 2, GL_FLOAT, 0, 0, textureCoordinates);

    GLuint currentTexture = sourceTexture;
    
    gpu_uint numberOfStageFramebuffers = stageFramebuffers_.size();
    for (gpu_uint currentStage = 0; currentStage < numberOfStageFramebuffers; currentStage++)
    {
        GLuint currentFramebuffer = stageFramebuffers_[currentStage];
        glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer);
        
        gpu_float_size currentStageSize = stageSizes_[currentStage];
        glViewport(0, 0, (int)currentStageSize.width, (int)currentStageSize.height);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, currentTexture);
        
        glUniform1i(filterInputTextureUniform_, 2);
        
        glUniform1f(texelWidthUniform_, 0.5 / currentStageSize.width);
        glUniform1f(texelHeightUniform_, 0.5 / currentStageSize.height);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        currentTexture = stageTextures_[currentStage];

//        gpu_uint totalBytesForImage = (int)currentStageSize.width * (int)currentStageSize.height * 4;
//        GLubyte *rawImagePixels2 = (GLubyte *)malloc(totalBytesForImage);
//        glReadPixels(0, 0, (int)currentStageSize.width, (int)currentStageSize.height, GL_RGBA, GL_UNSIGNED_BYTE, rawImagePixels2);
//        CGDataProviderRef dataProvider = CGDataProviderCreateWithData(NULL, rawImagePixels2, totalBytesForImage, NULL);
//        CGColorSpaceRef defaultRGBColorSpace = CGColorSpaceCreateDeviceRGB();
//
//        gpu_float currentRedTotal = 0.0f, currentGreenTotal = 0.0f, currentBlueTotal = 0.0f, currentAlphaTotal = 0.0f;
//        gpu_uint totalNumberOfPixels = totalBytesForImage / 4;
//        
//        for (gpu_uint currentPixel = 0; currentPixel < totalNumberOfPixels; currentPixel++)
//        {
//            currentRedTotal += (gpu_float)rawImagePixels2[(currentPixel * 4)] / 255.0f;
//            currentGreenTotal += (gpu_float)rawImagePixels2[(currentPixel * 4) + 1] / 255.0f;
//            currentBlueTotal += (gpu_float)rawImagePixels2[(currentPixel * 4 + 2)] / 255.0f;
//            currentAlphaTotal += (gpu_float)rawImagePixels2[(currentPixel * 4) + 3] / 255.0f;
//        }
//        
//        NSLog(@"Stage %d average image red: %f, green: %f, blue: %f, alpha: %f", currentStage, currentRedTotal / (gpu_float)totalNumberOfPixels, currentGreenTotal / (gpu_float)totalNumberOfPixels, currentBlueTotal / (gpu_float)totalNumberOfPixels, currentAlphaTotal / (gpu_float)totalNumberOfPixels);
//
//        
//        CGImageRef cgImageFromBytes = CGImageCreate((int)currentStageSize.width, (int)currentStageSize.height, 8, 32, 4 * (int)currentStageSize.width, defaultRGBColorSpace, kCGBitmapByteOrderDefault | kCGImageAlphaLast, dataProvider, NULL, NO, kCGRenderingIntentDefault);
//
//        UIImage *imageToSave = [UIImage imageWithCGImage:cgImageFromBytes];
//        
//        NSData *dataForPNGFile = UIImagePNGRepresentation(imageToSave);
//        
//        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
//        NSString *documentsDirectory = [paths objectAtIndex:0];
//        
//        NSString *imageName = [NSString stringWithFormat:@"AverageLevel%d.png", currentStage];
//        NSError *error = nil;
//        if (![dataForPNGFile writeToFile:[documentsDirectory stringByAppendingPathComponent:imageName] options:NSAtomicWrite error:&error])
//        {
//            return;
//        }
    }
}

void GPUImageAverageColor::prepareForImageCapture()
{
    preparedToCaptureImage_ = true;
}

void GPUImageAverageColor::setInputRotation(GPUImageRotationMode newInputRotation, gpu_int textureIndex)
{
    inputRotation_ = kGPUImageNoRotation;
}

void GPUImageAverageColor::extractAverageColorAtFrameTime(gpu_time frameTime)
{
    gpu_uint lastStageIndex = stageSizes_.size() - 1;
    gpu_float_size finalStageSize = stageSizes_[lastStageIndex];
    gpu_uint totalNumberOfPixels = gpu_roundf(finalStageSize.width * finalStageSize.height);
    
    if (rawImagePixels_ == NULL)
    {
        rawImagePixels_ = new GLubyte[totalNumberOfPixels * 4];
    }

    glReadPixels(0, 0, (int)finalStageSize.width, (int)finalStageSize.height, GL_RGBA, GL_UNSIGNED_BYTE, rawImagePixels_);
    
    gpu_uint redTotal = 0, greenTotal = 0, blueTotal = 0, alphaTotal = 0;
    gpu_uint byteIndex = 0;
    for (gpu_uint currentPixel = 0; currentPixel < totalNumberOfPixels; currentPixel++)
    {
        redTotal += rawImagePixels_[byteIndex++];
        greenTotal += rawImagePixels_[byteIndex++];
        blueTotal += rawImagePixels_[byteIndex++];
        alphaTotal += rawImagePixels_[byteIndex++];
    }
    
    gpu_float normalizedRedTotal = (gpu_float)redTotal / (gpu_float)totalNumberOfPixels / 255.0;
    gpu_float normalizedGreenTotal = (gpu_float)greenTotal / (gpu_float)totalNumberOfPixels / 255.0;
    gpu_float normalizedBlueTotal = (gpu_float)blueTotal / (gpu_float)totalNumberOfPixels / 255.0;
    gpu_float normalizedAlphaTotal = (gpu_float)alphaTotal / (gpu_float)totalNumberOfPixels / 255.0;

    /* TODO:
    if (_colorAverageProcessingFinishedBlock != NULL)
    {
        _colorAverageProcessingFinishedBlock(normalizedRedTotal, normalizedGreenTotal, normalizedBlueTotal, normalizedAlphaTotal, frameTime);
    }*/
}
