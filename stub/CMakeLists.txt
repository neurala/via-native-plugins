cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(stub SHARED
	src/demangle.cpp
	src/Option.cpp
	src/ErrorCode.cpp
	src/Exception.cpp
	src/PluginRegistrar.cpp)

target_include_directories(stub SYSTEM PUBLIC include)

target_compile_options(stub PUBLIC /permissive- /Zc:preprocessor /Zc:__cplusplus /MP /wd4251)

# TODO : use the correct define
target_compile_definitions(stub PRIVATE NeuralaB4B_EXPORTS)
target_compile_definitions(stub PRIVATE NEURALA_IMPORT_PLUGIN)
