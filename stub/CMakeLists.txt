# This CMake target will build a minimal library to link against
# that will provide the minimal dependencies to build a plugin

cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(NeuralaB4B SHARED
	src/B4BError.cpp
	src/demangle.cpp
	src/PluginStatus.cpp
	src/Options.cpp
	src/PluginManager.cpp
	src/VideoSourceStatus.cpp)

include(GenerateExportHeader)
generate_export_header(NeuralaB4B
	BASE_NAME ""
	PREFIX_NAME NEURALA_
	EXPORT_MACRO_NAME PUBLIC
	NO_EXPORT_MACRO_NAME PRIVATE
	DEPRECATED_MACRO_NAME DEPRECATED
	NO_DEPRECATED_MACRO_NAME NOT_DEPRECATED
	STATIC_DEFINE STATIC_LIB
	EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/neurala/exports.h)

target_compile_definitions(NeuralaB4B PRIVATE NeuralaB4B_EXPORTS)

target_include_directories(NeuralaB4B SYSTEM PUBLIC include ${CMAKE_BINARY_DIR}/include)

# The compiler options for this stub library are part of its public interface, so it will be propagated
# to all targets that depends on this stub by CMake.
# See: https://cmake.org/cmake/help/latest/prop_tgt/INTERFACE_COMPILE_OPTIONS.html
#
# /permissive- :  disables permissive behaviors, and sets the /Zc compiler options for strict conformance.
# /Zc:preprocessor : Use the new conforming preprocessor (off by default, except in C11/C17).
# /Zc:__cplusplus : Enable the __cplusplus macro to report the supported standard (off by default).
# /MP : Build with multiple processes
#
# Some of our classes with dll-interface inherit from STL classes that are not specified as such
# This is not an issue as long as the same compiler version is used for building the dll.
# C4251: 'type' : class 'type1' needs to have dll-interface to be used by clients of class 'type2'
#
# These two following warnings are disabled as they are produced by the Windows SDK,
# they have been fixed as of Windows 10 SDK version 2104 (10.0.20348.0).
#
# C5104: found 'L#x' in macro replacement list, did you mean 'L""#x'?
# Spammed by C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\shared\ntverp.h
# https://developercommunity.visualstudio.com/t/897227
#
# C5105: macro expansion producing 'defined' has undefined behavior
# Spammed by C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um\winbase.h
# https://developercommunity.visualstudio.com/t/1249671
#
if(MSVC)
	target_compile_options(NeuralaB4B PUBLIC /permissive- /Zc:preprocessor /Zc:__cplusplus /MP /wd4251 /wd5104 /wd5105)
endif()

# Add friendly alias
add_library(stub ALIAS NeuralaB4B)
