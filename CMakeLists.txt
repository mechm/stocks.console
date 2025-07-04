cmake_minimum_required(VERSION 3.10)

project(StocksConsole)

find_package(fmt CONFIG REQUIRED)

add_executable(stocks.console stocks.console.cpp)

target_link_libraries(stocks.console PRIVATE fmt::fmt)