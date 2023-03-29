cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

# Skip this target if the prepackaged dependencies have not been unpacked.
if(NOT EXISTS "external/CMSMultispectralLink.dll")
    message(WARNING "CMS files not present. Skipping CMS plugin.")
endif()

# For pulling in third party dependencies.
include(conan)

# Default to Release, including the case of RelWithDebInfo.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CONAN_BUILD_TYPE "Debug")
else()
    set(CONAN_BUILD_TYPE "Release")
endif()

# Pull Cereal as a JSON deserializer.
conan_cmake_configure(
    REQUIRES
        cereal/1.3.2@
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
         ${CONAN_CENTER} # Parsed from conan remote list in parent scope
    BUILD
        missing
    SETTINGS
        ${CONAN_SETTINGS}
)

include("${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup(TARGETS KEEP_PATHS NO_OUTPUT_DIRS)

add_library(external::cms INTERFACE IMPORTED GLOBAL)

# This project links against CMS, which depends on a fixed set of libraries and headers provided by SILIOS.
# In order to avoid breakage, these libraries are linked against verbatim, and are provided via Dropbox.
# Linux is not yet supported.
if(WIN32)
    set_target_properties(external::cms
        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/external"
            INTERFACE_LINK_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/external"
            INTERFACE_LINK_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/external/CMSMultispectralLink.lib;${CMAKE_CURRENT_LIST_DIR}/external/opencv_world331.lib")
    file(COPY "external/CMSMultispectralLink.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_library(cms SHARED src/cms.cpp)

target_compile_definitions(cms PRIVATE NEURALA_EXPORT_PLUGIN)
target_include_directories(cms PUBLIC include)
target_link_libraries(cms
    PUBLIC
        stub
    PRIVATE
        external::cms
        CONAN_PKG::cereal)
