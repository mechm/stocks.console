cmake_minimum_required(VERSION 3.15)
project(stocks.console)

set(CMAKE_CXX_STANDARD 20)

#find_package(fmt CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

add_executable(stocks.console main.cpp)

# Specify include directories for MyStaticLibrary
target_include_directories(stocks.console
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add the subdirectory. This will process third_party/my_dependency/CMakeLists.txt
#add_subdirectory(../stocks.console.api)
#add_subdirectory(../stocks.console.indicator)
#add_subdirectory(../stocks.console.utilities)

#target_link_libraries(stocks.console PRIVATE fmt::fmt)
target_link_libraries(stocks.console PRIVATE JsonCpp::JsonCpp)
#target_link_libraries(stocks.console PUBLIC stocks.console.indicator)
#target_link_libraries(stocks.console PUBLIC stocks.console.api)
#target_link_libraries(stocks.console PUBLIC stocks.console.utilities)

