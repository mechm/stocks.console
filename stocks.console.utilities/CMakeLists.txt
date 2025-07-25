cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

project(stocks.console.utilities.proj)

find_package(jsoncpp CONFIG REQUIRED)

set(MY_LIBRARY_SOURCES
    date_utilities.cpp
    formatter.cpp
    string_utilities.cpp
)

set(MY_LIBRARY_HEADERS
   date_utilities.h
   formatter.h
   string_utilities.h
)

add_library(stocks.console.utilities
    ${MY_LIBRARY_SOURCES}
    ${MY_LIBRARY_HEADERS} # Listing headers here can help IDEs with indexing
)

target_include_directories(stocks.console.utilities
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(stocks.console.utilities PRIVATE JsonCpp::JsonCpp)

# Link to other libraries if MyStaticLibrary depends on them
# target_link_libraries(MyStaticLibrary PUBLIC AnotherLibrary)

# Set compile definitions
# target_compile_definitions(MyStaticLibrary PUBLIC MY_DEFINE=1)