cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(InternalPluginStub INTERFACE)

target_include_directories(InternalPluginStub INTERFACE .)
