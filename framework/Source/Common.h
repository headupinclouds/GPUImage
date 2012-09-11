/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#ifndef _Common_h_
#define _Common_h_

#define GPUImageRotationSwapsWidthAndHeight(rotation) ((rotation) == kGPUImageRotateLeft || (rotation) == kGPUImageRotateRight || (rotation) == kGPUImageRotateRightFlipVertical)

typedef enum { 
    kGPUImageNoRotation, 
    kGPUImageRotateLeft, 
    kGPUImageRotateRight, 
    kGPUImageFlipVertical, 
    kGPUImageFlipHorizonal, 
    kGPUImageRotateRightFlipVertical, 
    kGPUImageRotate180 
} GPUImageRotationMode;

#define FloatSizeEqualToSize(s1,s2) (((s1).width == (s2).width) && ((s1).height == (s2).height))

// math stuff
#define LOG2 0.30102999566398119521373889472449
#define log2(x) (log(x)/LOG2)

#endif // _Common_h_