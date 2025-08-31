cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

project(stocks.console.fundamental.analysis.proj)

set(MY_LIBRARY_SOURCES
        src/quantative/eps.cpp
)

set(MY_LIBRARY_HEADERS
        include/quantative/eps.h
)

add_library(stocks.console.fundamental.analysis
        ${MY_LIBRARY_SOURCES}
        ${MY_LIBRARY_HEADERS} # Listing headers here can help IDEs with indexing
)

target_include_directories(stocks.console.fundamental.analysis
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
