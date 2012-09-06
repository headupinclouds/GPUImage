# - Try to find OpenGLES2 and EGL
#
# Once done this will define
#
#  OPENGLES2_FOUND              - system has OpenGLES2
#  OPENGLES2_INCLUDE_DIR        - the GLES2 include directory
#  OPENGLES2_LIBRARY            - the GLES2 library
#
#  EGL_FOUND                    - system has EGL
#  EGL_INCLUDE_DIR              - the EGL include directory
#  EGL_LIBRARY                  - the EGL library
#

find_path (OPENGLES2_INCLUDE_DIR GLES2/gl2.h)
find_library (OPENGLES2_LIBRARY NAMES GLESv2)

find_path (EGL_INCLUDE_DIR EGL/egl.h)
find_library (EGL_LIBRARY NAMES EGL)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (OpenGLES2 DEFAULT_MSG OPENGLES2_INCLUDE_DIR OPENGLES2_LIBRARY)
find_package_handle_standard_args (EGL DEFAULT_MSG EGL_INCLUDE_DIR EGL_LIBRARY)

mark_as_advanced (OPENGLES2_INCLUDE_DIR OPENGLES2_LIBRARY EGL_INCLUDE_DIR EGL_LIBRARY)