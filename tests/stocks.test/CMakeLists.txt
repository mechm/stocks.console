cmake_minimum_required(VERSION 3.15)
project(stocks.test)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB MY_LIBRARY_HEADERS
        "include/*.h"
)

file(GLOB_RECURSE STOCKS_CONSOLE_SOURCES
        "src/indicator/*.cpp"
        "src/*.cpp"
        "src/fundamental.analysis/*.cpp"
)

add_executable(Google_Tests_run
        ${STOCKS_CONSOLE_SOURCES}
        ${MY_LIBRARY_HEADERS}
)

target_link_libraries(Google_Tests_run PRIVATE stocks.console.indicator)
target_link_libraries(Google_Tests_run PRIVATE stocks.console.fundamental.analysis)
target_link_libraries(Google_Tests_run PRIVATE gtest gtest_main)