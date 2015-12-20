cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

include("CMake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.12.33.tar.gz"
    SHA1 "f64eda11326e12d84b6aa7f4aed80c09c182aa64"
)

project(GPUImageCXX VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

# See: cmake/Hunter/config.cmake
hunter_add_package(Sugar)
include("${SUGAR_ROOT}/cmake/Sugar")
include(sugar_include)

## #################################################################
## Dependencies - OpenGL stuff
## #################################################################

if(IOS OR ANDROID)
  set(GATHERER_OPENGL_ES TRUE)
else()
  set(GATHERER_OPENGL_ES FALSE)
endif()

if(NOT GATHERER_OPENGL_ES)
  find_package(OpenGL REQUIRED)
endif()

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

###
# Build Examples
###
option(BUILD_EXAMPLES "Build the examples" OFF)

sugar_include(${CMAKE_SOURCE_DIR})

add_library(GPUImageCXX ${GPUIMAGE_FRAMEWORK_SRCS} ${GPUIMAGE_FRAMEWORK_PUBLIC_HDRS})

target_link_libraries(GPUImageCXX ${EGL_LIBRARY} ${OPENGLES2_LIBRARY})

if(ANDROID)
  target_link_libraries(GPUImageCXX EGL GLESv2)
endif()

if(IOS OR is_ios)
  # TODO: reduce copy and paste to minimum required set
  target_link_libraries(GPUImageCXX 
  "-framework CoreGraphics"
  "-framework Foundation"
  "-framework UIKit"
  "-framework QuartzCore"
  "-framework ImageIO"
  "-framework CoreGraphics"
  "-framework CoreVideo"
  "-framework CoreMedia"
  "-framework CoreLocation" 
  "-framework AVFoundation"
  "-framework GLKit"
  "-framework OpenGLES"
  "-framework UIKit"
  )
endif()

##################################################################
########################### Install ##############################
##################################################################

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")

set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Note: use 'targets_export_name'
configure_file("CMake/Config.cmake.in" "${project_config}" @ONLY)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}/${PROJECT_NAME}"
)

install(
  FILES
  ${GPUIMAGE_FRAMEWORK_PUBLIC_HDRS}
  DESTINATION "${include_install_dir}/${PROJECT_NAME}/common"
)

install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)
