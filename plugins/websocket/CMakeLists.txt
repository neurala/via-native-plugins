cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

include(conan)

# Default to Release, including the case of RelWithDebInfo.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CONAN_BUILD_TYPE "Debug")
else()
	set(CONAN_BUILD_TYPE "Release")
endif()

# Load Boost libraries through Conan.
conan_cmake_run(
	REQUIRES
		boost/1.76.0
	BASIC_SETUP
		CMAKE_TARGETS
		KEEP_RPATHS
		NO_OUTPUT_DIRS
	BUILD_TYPE
		${CONAN_BUILD_TYPE}
	BUILD
		missing
	OUTPUT_QUIET)

add_library(websocket SHARED
	src/Client.cpp
	src/InitMe.cpp
	src/Input.cpp
	src/InputServer.cpp
	src/OutputServer.cpp
	src/Server.cpp)

target_include_directories(websocket
	# Headers should be prefixed with websocket/ when used as a dependency.
	INTERFACE
		include
	PRIVATE
		include/websocket)

target_link_libraries(websocket PUBLIC stub CONAN_PKG::boost)

add_subdirectory(test)
add_subdirectory(tools)
