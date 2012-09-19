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
#define FloatSizeEqualToZero(s1) (((s1).width == 0.0f) && ((s1).height == 0.0f))

// math stuff
#define LOG2_CONSTANT 0.69314718055994529
#define gpuimage_log2(x) (log(x)/LOG2_CONSTANT)

#define gpu_roundf(x) (floor((x) + 0.5f))

#endif // _Common_h_