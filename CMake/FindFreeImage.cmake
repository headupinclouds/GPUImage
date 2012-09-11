# - Try to find the FreeImage library
#
# Once done this will define
#
#  FREEIMAGE_FOUND              - system has FreeImage
#  FREEIMAGE_INCLUDE_DIR        - the FreeImage include directory
#  FREEIMAGE_LIBRARY            - the FreeImage library
#

find_path (FREEIMAGE_INCLUDE_DIR FreeImage.h)
find_library (FREEIMAGE_LIBRARY NAMES FreeImage freeimage)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (FreeImage DEFAULT_MSG FREEIMAGE_INCLUDE_DIR FREEIMAGE_LIBRARY)

mark_as_advanced (FREEIMAGE_INCLUDE_DIR FREEIMAGE_LIBRARY)