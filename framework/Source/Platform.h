/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */
 
#ifndef _Platform_h_
#define _Platform_h_
 
// It would be better to use BOOST cstdint
typedef int gpu_int;
typedef unsigned int gpu_uint;
typedef float gpu_float;
typedef double gpu_double;
 
typedef struct {
    gpu_float x;
    gpu_float y;
} gpu_float_point;

typedef struct {
    gpu_float width;
    gpu_float height;
} gpu_float_size;

// https://developer.apple.com/library/ios/#DOCUMENTATION/AudioVideo/Conceptual/AVFoundationPG/Articles/05_MediaRepresentations.html ?
struct gpu_time {

};
 
#endif