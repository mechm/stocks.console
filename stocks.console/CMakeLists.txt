cmake_minimum_required(VERSION 3.15)
project(stocks.console)

set(CMAKE_CXX_STANDARD 20)

find_package(jsoncpp CONFIG REQUIRED)

add_executable(stocks.console main.cpp)

# Specify include directories for MyStaticLibrary
target_include_directories(stocks.console
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(stocks.console PRIVATE JsonCpp::JsonCpp)