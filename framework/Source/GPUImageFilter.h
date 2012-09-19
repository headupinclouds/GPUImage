/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _GPUImageFilter_h_
#define _GPUImageFilter_h_

#include "GPUImageOutput.h"
#include "GPUImageInput.h"
#include <string>

#define STRINGIZE(x) #x
#define STRINGIZE2(x) STRINGIZE(x)
#define SHADER_STRING(text) @ STRINGIZE2(text)

//extern NSString *const kGPUImageVertexShaderString;
//extern NSString *const kGPUImagePassthroughFragmentShaderString;

typedef struct {
    GLfloat one;
    GLfloat two;
    GLfloat three;
    GLfloat four;
} GPUVector4;

typedef struct {
    GLfloat one;
    GLfloat two;
    GLfloat three;
} GPUVector3;

typedef struct {
    GPUVector4 one;
    GPUVector4 two;
    GPUVector4 three;
    GPUVector4 four;
} GPUMatrix4x4;

typedef struct {
    GPUVector3 one;
    GPUVector3 two;
    GPUVector3 three;
} GPUMatrix3x3;

// forward declarations
class GLProgram;

/** GPUImage's base filter class
 
 Filters and other subsequent elements in the chain conform to the GPUImageInput protocol, which lets them take in the supplied or processed texture from the previous link in the chain and do something with it. Objects one step further down the chain are considered targets, and processing can be branched by adding multiple targets to a single output or filter.
 */
class GPUImageFilter : public GPUImageOutput, public GPUImageInput 
{
public:
    GPUImageFilter();
    virtual ~GPUImageFilter();

    /// @name Initialization and teardown

    /**
     Initialize with vertex and fragment shaders
     
     You make take advantage of the SHADER_STRING macro to write your shaders in-line.
     @param vertexShaderString Source code of the vertex shader to use
     @param fragmentShaderString Source code of the fragment shader to use
     */
    virtual void initWithVertexShaderFromString(const std::string& vertexShaderString, const std::string& fragmentShaderString);

    /**
     Initialize with a fragment shader
     
     You may take advantage of the SHADER_STRING macro to write your shader in-line.
     @param fragmentShaderString Source code of fragment shader to use
     */
    virtual void initWithFragmentShaderFromString(const std::string& fragmentShaderString);

    /**
     Initialize with a fragment shader
     @param fragmentShaderFilename Filename of fragment shader to load
     */
    virtual void initWithFragmentShaderFromFile(const std::string& fragmentShaderFilename);

    void initializeAttributes();
    virtual void setupFilterForSize(gpu_float_size filterFrameSize);
    virtual gpu_float_size rotatedSize(gpu_float_size sizeToRotate, gpu_int textureIndex);
    gpu_float_point rotatedPoint(gpu_float_point pointToRotate, GPUImageRotationMode rotation);

    void recreateFilterFBO();

    /// @name Managing the display FBOs
    /** Size of the frame buffer object
     */
    gpu_float_size sizeOfFBO();
    virtual void createFilterFBOofSize(gpu_float_size currentFBOSize);

    /** Destroy the current filter frame buffer object
     */
    virtual void destroyFilterFBO();
    void setFilterFBO();
    virtual void setOutputFBO();

    /// @name Rendering
    static const GLfloat* textureCoordinatesForRotation(GPUImageRotationMode rotationMode);
    virtual void renderToTextureWithVertices(const GLfloat* vertices, const GLfloat* textureCoordinates, GLuint sourceTexture);
    virtual void setUniformsForProgramAtIndex(gpu_uint programIndex);
    void informTargetsAboutNewFrameAtTime(gpu_time frameTime);
    gpu_float_size getOutputFrameSize();

    /// @name Input parameters
    void setBackgroundColor(GLfloat redComponent, GLfloat greenComponent, GLfloat blueComponent, GLfloat alphaComponent);
    void setInteger(GLint newInteger, const std::string& uniformName);
    void setFloat(GLfloat newFloat, const std::string& uniformName);
    void setSize(gpu_float_size newSize, const std::string& uniformName);
    void setPoint(gpu_float_point newPoint, const std::string& uniformName);
    void setFloatVec3(GPUVector3 newVec3, const std::string& uniformName);
    void setFloatVec4(GPUVector4 newVec4, const std::string& uniformName);
    void setFloatArray(GLfloat* floatArray, GLsizei count, const std::string& uniformName);

    void setMatrix3f(GPUMatrix3x3 matrix, GLint uniform, GLProgram* shaderProgram);
    void setMatrix4f(GPUMatrix4x4 matrix, GLint uniform, GLProgram* shaderProgram);
    void setFloat(GLfloat floatValue, GLint uniform, GLProgram* shaderProgram);
    void setPoint(gpu_float_point pointValue, GLint uniform, GLProgram* shaderProgram);
    void setSize(gpu_float_size sizeValue, GLint uniform, GLProgram* shaderProgram);
    void setVec3(GPUVector3 vectorValue, GLint uniform, GLProgram* shaderProgram);
    void setVec4(GPUVector4 vectorValue, GLint uniform, GLProgram* shaderProgram);
    void setFloatArray(GLfloat* arrayValue, GLsizei arrayLength, GLint uniform, GLProgram* shaderProgram);
    void setInteger(GLint intValue, GLint uniform, GLProgram* shaderProgram);

    void newFrameReadyAtTime(gpu_time frameTime, gpu_int textureIndex);
    void setInputTexture(GLuint newInputTexture, gpu_int textureIndex);
    gpu_int nextAvailableTextureIndex();
    void setInputSize(gpu_float_size newSize, gpu_int textureIndex);
    void setInputRotation(GPUImageRotationMode newInputRotation, gpu_int textureIndex);
    gpu_float_size maximumOutputSize();
    void endProcessing();

    bool shouldIgnoreUpdatesToThisTarget();
    void setShouldIgnoreUpdatesToThisTarget(bool ignore);
    bool enabled();
    void setEnabled(bool enable);

    static const std::string kGPUImageVertexShaderString;
    static const std::string kGPUImagePassthroughFragmentShaderString;

protected: 
    GLProgram *filterProgram_;
    GPUImageRotationMode inputRotation_;

    bool preventRendering_;
    
    GLuint filterSourceTexture_;

    GLint filterPositionAttribute_; 
    GLint filterTextureCoordinateAttribute_;
    GLint filterInputTextureUniform_;

    GLfloat backgroundColorRed_, backgroundColorGreen_, backgroundColorBlue_, backgroundColorAlpha_;

    bool preparedToCaptureImage_;

private:

    GLuint filterFramebuffer_;
        
    //CVOpenGLESTextureCacheRef filterTextureCache_;
    //CVPixelBufferRef renderTarget_;
    //CVOpenGLESTextureRef renderTexture_;
    
    gpu_float_size currentFilterSize_;

    //CVPixelBufferRef renderTarget_;

    bool shouldIgnoreUpdatesToThisTarget_;
    bool enabled_;
};

#endif // _GPUImageFilter_h_
