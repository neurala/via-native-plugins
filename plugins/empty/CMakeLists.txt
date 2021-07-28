# This CMake target will build a simple no-op plugin

cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

# Build the plugin
add_library(empty SHARED src/initme.cpp)
target_include_directories(empty PUBLIC include)
target_link_libraries(empty stub)

# Build the tests
add_executable(empty_tests test/main.cpp)
target_link_libraries(empty_tests empty)
target_compile_definitions(empty_tests PRIVATE NEURALA_IMPORT_PLUGIN)
