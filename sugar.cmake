# This file generated automatically by:
#   generate_sugar_files.py
# see wiki for more info:
#   https://github.com/ruslo/sugar/wiki/Collecting-sources

if(DEFINED GPUIMAGE_SUGAR_CMAKE_)
  return()
else()
  set(GPUIMAGE_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)
include(sugar_include)

sugar_include(framework) # GPUIMAGE_FRAMEWORK_SRCS

if(BUILD_TESTS)
  sugar_include(tests)     # GPUIMAGE_TEST_SRCS
endif()

if(BUILD_EXAMPLES)
  sugar_include(examples)  # GPUIMAGE_EXAMPLES_SRCS
endif()
