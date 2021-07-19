# This CMake target will build a simple no-op plugin

cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

# Build the plugin
set(plugin_target empty)
add_library(${plugin_target} SHARED src/initme.cpp)
target_include_directories(${plugin_target} PUBLIC include)
target_link_libraries(${plugin_target} stub)

# Build the tests
set(test_target ${plugin_target}_tests)
add_executable(${test_target} test/main.cpp)
target_link_libraries(${test_target} ${plugin_target})
