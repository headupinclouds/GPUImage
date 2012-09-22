/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImageMaskFilter.h"

const std::string GPUImageMaskFilter::kGPUImageMaskShaderString("\
 varying highp vec2 textureCoordinate;\n\
 varying highp vec2 textureCoordinate2;\n\
 \n\
 uniform sampler2D inputImageTexture;\n\
 uniform sampler2D inputImageTexture2;\n\
 \n\
 void main()\n\
 {\n\
	 lowp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n\
	 lowp vec4 textureColor2 = texture2D(inputImageTexture2, textureCoordinate2);\n\
	 \n\
	 //Averages mask's the RGB values, and scales that value by the mask's alpha\n\
	 //\n\
	 //The dot product should take fewer cycles than doing an average normally\n\
	 //\n\
	 //Typical/ideal case, R,G, and B will be the same, and Alpha will be 1.0\n\
	 lowp float newAlpha = dot(textureColor2.rgb, vec3(.33333334, .33333334, .33333334)) * textureColor2.a;\n\
	 \n\
	 gl_FragColor = vec4(textureColor.xyz, newAlpha);\n\
//	 gl_FragColor = vec4(textureColor2);\n\
 }\n"
);

GPUImageMaskFilter::GPUImageMaskFilter() :
    GPUImageTwoInputFilter() {

        GPUImageTwoInputFilter::initWithFragmentShaderFromString(kGPUImageMaskShaderString);
}

GPUImageMaskFilter::~GPUImageMaskFilter() {

}

void GPUImageMaskFilter::renderToTextureWithVertices(const GLfloat* vertices, const GLfloat* textureCoordinates, GLuint sourceTexture) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GPUImageTwoInputFilter::renderToTextureWithVertices(vertices, textureCoordinates, sourceTexture);
    glDisable(GL_BLEND);
}

