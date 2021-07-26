# This CMake target will build a dummy plugin with basic input/outputs
# that also supports automatic discovery.
# To enable automatic camera discovery, the registered CameraDiscoverer class
# should have the name of the plugin (without extension) followed by "Discoverer"
# e.g. dummyPlugin.dll -> dummyPluginDiscoverer

cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

# Build the plugin
set(plugin_target dummy)
add_library(${plugin_target} SHARED src/dummy.cpp)
target_include_directories(${plugin_target} PUBLIC include)
target_link_libraries(${plugin_target} NeuralaB4B)

# Build the tests
set(test_target ${plugin_target}_tests)
add_executable(${test_target} test/main.cpp)
target_link_libraries(${test_target} ${plugin_target})
target_compile_definitions(${test_target} PRIVATE NEURALA_IMPORT_PLUGIN)
