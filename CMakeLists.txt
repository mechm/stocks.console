cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

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

target_link_libraries(stocks.console PRIVATE JsonCpp::JsonCpp)
target_link_libraries(stocks.console PRIVATE stocks.console.indicator)
target_link_libraries(stocks.console PRIVATE stocks.console.api)
target_link_libraries(stocks.console PRIVATE stocks.console.utilities)