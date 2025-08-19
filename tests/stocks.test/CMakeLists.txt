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

add_executable(Google_Tests_run RSITest.cpp)

target_link_libraries(Google_Tests_run PRIVATE stocks.console.indicator)
target_link_libraries(Google_Tests_run PRIVATE gtest gtest_main)