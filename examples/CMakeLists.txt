option(BUILD_EXAMPLES "Build the library examples" FALSE)

if(BUILD_EXAMPLES)
    
    include_directories(${CMAKE_SOURCE_DIR}/framework/Source)
    
    #
    add_executable(gputest GPUTest/GPUTest.cpp)
    target_link_libraries(gputest GPUImage)

endif(BUILD_EXAMPLES)