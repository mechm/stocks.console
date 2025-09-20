cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(stocks.console)

# config.json doesn't need CMake variable substitution
set(CONFIG_JSON_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/stocks.console/config_doc.json")

#set(EXECUTABLE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<IF:$<CONFIG>,$<CONFIG>,.>") # For VS: build/Debug, build/Release. For Make: build/.
set(EXECUTABLE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Debug") # For VS: build/Debug, build/Release. For Make: build/.

# Define the full path to the destination of the config file
set(CONFIG_JSON_DESTINATION "${EXECUTABLE_OUTPUT_DIR}/config_doc.json")

# Copy the file to the build directory.
add_custom_command(
        OUTPUT "${CONFIG_JSON_DESTINATION}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CONFIG_JSON_SOURCE}"
        "${CONFIG_JSON_DESTINATION}"
        DEPENDS "${CONFIG_JSON_SOURCE}"
        COMMENT "Copying config_doc.json to executable output directory"
)

add_custom_target(CopyConfigJson ALL DEPENDS "${CONFIG_JSON_DESTINATION}")

find_package(jsoncpp CONFIG REQUIRED)
#find_package(libpq CONFIG REQUIRED)
#find_package(pqxx CONFIG REQUIRED)
#find_package(libpqxx CONFIG REQUIRED)
#find_package(libpqxx CONFIG REQUIRED)
#find_package(pqxx CONFIG REQUIRED)
#find_package(pqxx CONFIG REQUIRED)
#find_package(libpqxx CONFIG REQUIRED)
#find_package(pqxx CONFIG REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

set(PQXX /usr/local/include/pqxx)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

# Automatically collect all source and header files
file(GLOB_RECURSE STOCKS_CONSOLE_SOURCES
    "stocks.console/src/*.cpp"
    "stocks.console/include/*.h"
)

# Build your executable
add_executable(stocks.console ${STOCKS_CONSOLE_SOURCES}
        stocks.console/include/indicator/macd.h)

add_dependencies(stocks.console CopyConfigJson)

# Add the subdirectory. This will process third_party/my_dependency/CMakeLists.txt
add_subdirectory(stocks.console.api)
add_subdirectory(stocks.console.indicator)
add_subdirectory(stocks.console.utilities)
add_subdirectory(stocks.console.fundamental.analysis)
add_subdirectory(tests/stocks.test)

target_link_libraries(stocks.console PRIVATE
        JsonCpp::JsonCpp
        ${PQXX_LIB} ${PQ_LIB}
        stocks.console.api
        stocks.console.indicator
        stocks.console.utilities
        stocks.console.fundamental.analysis
)