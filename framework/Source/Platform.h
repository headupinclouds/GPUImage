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
 
struct gpu_float_size {
    gpu_float width;
    gpu_float height;
};

// https://developer.apple.com/library/ios/#DOCUMENTATION/AudioVideo/Conceptual/AVFoundationPG/Articles/05_MediaRepresentations.html ?
struct gpu_time {

};
 
#endif