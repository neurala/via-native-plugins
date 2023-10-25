# This CMake target will build a dummy plugin with basic input/outputs
# that also supports automatic discovery.
# To enable automatic camera discovery, the registered CameraDiscoverer class
# should have the name of the plugin (without extension) followed by "Discoverer"
# e.g. dummyPlugin.dll -> dummyPluginDiscoverer

cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

# Build the plugin
add_library(dummy SHARED src/dummy.cpp)
set_target_properties(dummy PROPERTIES PREFIX "")
target_compile_definitions(dummy PRIVATE NEURALA_EXPORT_PLUGIN)
target_include_directories(dummy PUBLIC include)
target_link_libraries(dummy stub)

# Build the tests
add_executable(dummy_tests test/main.cpp)
target_link_libraries(dummy_tests dummy)
