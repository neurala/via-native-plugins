cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

include(conan)

conan_cmake_configure(
	REQUIRES
		opencv/4.5.3@
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
		conan-center
	BUILD
		missing
	SETTINGS
		${CONAN_SETTINGS}
)

include("${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake")

conan_basic_setup(TARGETS KEEP_PATHS NO_OUTPUT_DIRS)

add_library(external::cms INTERFACE IMPORTED GLOBAL)

if(WIN32)
    get_target_property(OPENCV_INCLUDES CONAN_PKG::opencv INTERFACE_INCLUDE_DIRECTORIES)
    set_target_properties(external::cms
        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/external;${OPENCV_INCLUDES}"
            INTERFACE_LINK_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/external"
            INTERFACE_LINK_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/external/CMSMultispectralLink.lib;CONAN_PKG::opencv")
    file(COPY "external/CMSMultispectralLink.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_library(cms SHARED src/cms.cpp)

target_compile_definitions(cms PRIVATE NEURALA_EXPORT_PLUGIN)
target_include_directories(cms PUBLIC include)
target_link_libraries(cms
    PUBLIC
        stub
    PRIVATE
        external::cms)
