cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

project(stocks.console.api.proj)

find_package(CURL CONFIG REQUIRED)

set(MY_LIBRARY_SOURCES
   alpacha.cpp
   http.cpp
)

set(MY_LIBRARY_HEADERS
   alpacha.h
   http.h
)

add_library(stocks.console.api
    ${MY_LIBRARY_SOURCES}
    ${MY_LIBRARY_HEADERS} # Listing headers here can help IDEs with indexing
)

target_include_directories(stocks.console.api
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(stocks.console.api PRIVATE CURL::libcurl)