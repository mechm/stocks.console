cmake_minimum_required(VERSION 3.15)
project(stocks.test)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/googletest)

# Use a unique binary directory for stocks.console.indicator to avoid conflicts
#add_subdirectory(
#    ../../stocks.console.indicator
#    ${CMAKE_BINARY_DIR}/stocks_console_indicator_build
#)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Find Google Test's main library target
#find_package(GTest REQUIRED)

add_executable(Google_Tests_run RSITest.cpp
        indicator/rsi_test.cpp
)

target_link_libraries(Google_Tests_run PRIVATE stocks.console.indicator)
target_link_libraries(Google_Tests_run PRIVATE gtest gtest_main)

# Enable testing for the project
enable_testing() 

# Add the tests to be run by CTest
# This command tells CMake to find tests and add them to the CTest suite
#include(GoogleTest)
#gtest_discover_tests(MyTests)