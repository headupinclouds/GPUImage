cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(GPUImage)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(OpenGLES2)
find_package(GTest)

include_directories(${EGL_INCLUDE_DIR} ${OPENGLES2_INCLUDE_DIR})

###
# Build Tests
###
add_subdirectory(tests)

###
# Build Examples
###
add_subdirectory(examples)

add_library(GPUImage STATIC framework/Source/GPUImageOpenGLESContext.cpp)
target_link_libraries(GPUImage ${EGL_LIBRARY} ${OPENGLES2_LIBRARY})
