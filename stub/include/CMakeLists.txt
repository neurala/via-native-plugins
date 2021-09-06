cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(plugin_headers INTERFACE)

target_include_directories(plugin_headers INTERFACE .)

add_library(neurala::plugin_headers ALIAS plugin_headers)
