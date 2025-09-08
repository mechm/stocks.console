cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

project(stocks.console.indicator)

# Dynamically collect all .cpp source files in src/
file(GLOB MY_LIBRARY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Dynamically collect all .h header files in include/
file(GLOB MY_LIBRARY_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)


add_library(stocks.console.indicator
    ${MY_LIBRARY_SOURCES}
    ${MY_LIBRARY_HEADERS} # Listing headers here can help IDEs with indexing
)

target_include_directories(stocks.console.indicator
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
