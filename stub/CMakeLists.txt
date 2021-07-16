cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(stub SHARED
	src/demangle.cpp
	src/ErrorCode.cpp
	src/Exception.cpp
	src/Option.cpp
	src/PluginRegistrar.cpp
	src/ResultsOutput.cpp)

# TODO : copy only the required header files
target_include_directories(stub SYSTEM PUBLIC
	include # Local include directory
	../../../include # B4B include directory
	../../../../../build/msvc-release/include # NEUWeb exported headers (for neurala/exports.h)
	../../../../../include # NEUWeb include directory
)

target_compile_options(stub PUBLIC /permissive- /Zc:preprocessor /Zc:__cplusplus /MP /wd4251)

# TODO : use the correct define
target_compile_definitions(stub PRIVATE NeuralaB4B_EXPORTS)
