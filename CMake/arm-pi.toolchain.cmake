# Cross-Compilation tool chain file for CMake
# for use with Windows and Cygwin with an ARM tool chain
#
# Author: Alessio Placitelli
# Contact: a.placitelli _AT_ a2p.it
#

# this one is important
set(CMAKE_SYSTEM_NAME Linux)

# this one not so much
set(CMAKE_SYSTEM_VERSION 1)

# base path for the toolchain. Build other paths on the top of it.
set(ARMRPI_TOOLCHAIN_ROOT /opt/cross/x-tools/arm-unknown-linux-gnueabi)

# let the build system know we are building for RPI
set(RPI_BUILD enabled CACHE INTERNAL STRING FORCE)

if(NOT EXISTS ${ARMRPI_TOOLCHAIN_ROOT})
  message(FATAL_ERROR " ${ARMRPI_TOOLCHAIN_ROOT} does not exist or not found!")
endif()

# specify the cross compiler
set(CMAKE_C_COMPILER   ${ARMRPI_TOOLCHAIN_ROOT}/bin/arm-unknown-linux-gnueabi-gcc.exe      CACHE PATH "gcc" FORCE)
set(CMAKE_CXX_COMPILER ${ARMRPI_TOOLCHAIN_ROOT}/bin/arm-unknown-linux-gnueabi-g++.exe      CACHE PATH "g++" FORCE)
set(CMAKE_AR           ${ARMRPI_TOOLCHAIN_ROOT}/bin/arm-unknown-linux-gnueabi-ar.exe       CACHE PATH "archive" FORCE)
set(CMAKE_LINKER       ${ARMRPI_TOOLCHAIN_ROOT}/bin/arm-unknown-linux-gnueabi-ld.exe       CACHE PATH "linker" FORCE)
set(CMAKE_NM           ${ARMRPI_TOOLCHAIN_ROOT}/bin/arm-unknown-linux-gnueabi-nm.exe       CACHE PATH "nm" FORCE)
set(CMAKE_OBJCOPY      ${ARMRPI_TOOLCHAIN_ROOT}/bin/arm-unknown-linux-gnueabi-objcopy.exe  CACHE PATH "objcopy" FORCE)
set(CMAKE_OBJDUMP      ${ARMRPI_TOOLCHAIN_ROOT}/bin/arm-unknown-linux-gnueabi-objdump.exe  CACHE PATH "objdump" FORCE)
set(CMAKE_STRIP        ${ARMRPI_TOOLCHAIN_ROOT}/bin/arm-unknown-linux-gnueabi-strip.exe    CACHE PATH "strip" FORCE)
set(CMAKE_RANLIB       ${ARMRPI_TOOLCHAIN_ROOT}/bin/arm-unknown-linux-gnueabi-ranlib.exe   CACHE PATH "ranlib" FORCE)

# where is the target environment 
set(CMAKE_FIND_ROOT_PATH  ${ARMRPI_TOOLCHAIN_ROOT})

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)