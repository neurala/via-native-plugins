cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

add_library(stub SHARED
	src/demangle.cpp
	src/ErrorCode.cpp
	src/Exception.cpp
	src/Option.cpp
	src/PluginRegistrar.cpp)

target_include_directories(pluginsStub PUBLIC include)
	
# TODO : copy only the required header files
target_include_directories(pluginsStub SYSTEM PRIVATE
	../../include
	../../build/msvc-release/include)

target_compile_options(pluginsStub PRIVATE /permissive- /Zc:preprocessor /Zc:__cplusplus /MP /wd4251)

# TODO : use the correct define
target_compile_definitions(pluginsStub PRIVATE NeuralaB4B_EXPORTS)
