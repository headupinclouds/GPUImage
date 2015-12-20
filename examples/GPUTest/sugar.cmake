# This file generated automatically by:
#   generate_sugar_files.py
# see wiki for more info:
#   https://github.com/ruslo/sugar/wiki/Collecting-sources

if(DEFINED EXAMPLES_GPUTEST_SUGAR_CMAKE_)
  return()
else()
  set(EXAMPLES_GPUTEST_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)

sugar_files(
    GPUIMAGE_EXAMPLES_SRCS
    GPUTest.cpp
    sugar.cmake~
)
