cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

project(stocks.console.indicator)

set(MY_LIBRARY_SOURCES
   macd.cpp
   rsi.cpp
   sma.cpp
)

set(MY_LIBRARY_HEADERS
   macd.h
   rsi.h
   sma.h
)

add_library(stocks.console.indicator
    ${MY_LIBRARY_SOURCES}
    ${MY_LIBRARY_HEADERS} # Listing headers here can help IDEs with indexing
)

target_include_directories(stocks.console.indicator
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
