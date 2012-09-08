if(GTEST_FOUND)
    set(BUILD_TESTS_ENABLED_DEFAULT ON)
else(GTEST_FOUND)
    set(BUILD_TESTS_ENABLED_DEFAULT OFF)
endif(GTEST_FOUND)

option(BUILD_TESTS "Build the library tests" ${BUILD_TESTS_ENABLED_DEFAULT})

# If we build the tests, enable the testing subsystem
if(BUILD_TESTS)
    enable_testing()
    include_directories(${GTEST_INCLUDE_DIRS})
    
    # Add test cpp file
    add_executable(runUnitTests testgtest.cpp)
    
    # Link test executable against gtest & gtest_main
    target_link_libraries(runUnitTests ${GTEST_BOTH_LIBRARIES})
    add_test(runUnitTests runUnitTests)    
endif(BUILD_TESTS)