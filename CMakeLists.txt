cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(GPUImageCXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(OpenGLES2)

# FreeImage is used minimally in the originally branch and is easily removed
#find_package(FreeImage)

find_package(GTest)

include_directories(${EGL_INCLUDE_DIR} ${OPENGLES2_INCLUDE_DIR})
include_directories(${FREEIMAGE_INCLUDE_DIR})

###
#	This is needed for RPi because of this issue:
#	https://github.com/raspberrypi/firmware/issues/34
###
if (RPI_BUILD)
  find_path (VCOS_PLATFORM_TYPES_PATH vcos_platform_types.h)
  include_directories(${VCOS_PLATFORM_TYPES_PATH})
endif()

###
# Build Tests
###
option(BUILD_TESTS "Build the tests" OFF)
if(BIULD_TESTS)
  add_subdirectory(tests)
endif()

###
# Build Examples
###
option(BUILD_EXAMPLES "Build the examples" OFF)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# Find all the input files
FILE(GLOB GPUIMAGE_SOURCE_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/framework/Source/*.cpp")
FILE(GLOB GPUIMAGE_HEADER_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/framework/Source/*.h")

list(REMOVE_ITEM lib_srcs "IlmImf/b44ExpLogTable.cpp"

# Organize the files in the project
source_group("Headers" FILES ${GPUIMAGE_HEADER_FILES})

add_library(GPUImageCXX STATIC ${GPUIMAGE_SOURCE_FILES} ${GPUIMAGE_HEADER_FILES})
target_link_libraries(GPUImage ${EGL_LIBRARY} ${OPENGLES2_LIBRARY})
target_link_libraries(GPUImage ${FREEIMAGE_LIBRARY})
