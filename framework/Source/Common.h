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

#endif // _Common_h_