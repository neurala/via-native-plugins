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
conan_cmake_configure(
	REQUIRES
		boost/1.76.0@
		libjpeg-turbo/2.1.2@
	OPTIONS
		libjpeg-turbo:shared=False
	GENERATORS
		"cmake_find_package"
		"cmake"
	IMPORTS
		"bin, *.dll -> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
		"lib, *.dynlib* -> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
		"lib, *.so* -> ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
conan_cmake_autodetect(CONAN_SETTINGS BUILD_TYPE "${CONAN_BUILD_TYPE}")
conan_cmake_install(
	PATH_OR_REFERENCE .
	UPDATE
	REMOTE
		conancenter
	BUILD
		missing
	SETTINGS
		${CONAN_SETTINGS}
)

include("${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup(TARGETS KEEP_PATHS NO_OUTPUT_DIRS)

add_library(websocket SHARED
	src/Client.cpp
	src/InitMe.cpp
	src/Input.cpp
	src/JPG.cpp)

target_compile_definitions(websocket PRIVATE NEURALA_EXPORT_PLUGIN)

target_include_directories(websocket PUBLIC include)

target_link_libraries(websocket PUBLIC stub CONAN_PKG::boost CONAN_PKG::libjpeg-turbo)

add_subdirectory(servers)
add_subdirectory(test)
