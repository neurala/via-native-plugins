cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

include(conan)

# Default to Release, including the case of RelWithDebInfo.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CONAN_BUILD_TYPE "Debug")
else()
    set(CONAN_BUILD_TYPE "Release")
endif()

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
    set_target_properties(external::cms
        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/external"
            INTERFACE_LINK_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/external"
            INTERFACE_LINK_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/external/CMSMultispectralLink.lib;${CMAKE_CURRENT_LIST_DIR}/external/opencv_world331.lib")
    file(COPY "external/CMSMultispectralLink.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_library(cms SHARED src/cms.cpp)
add_executable(cms_test src/test.cpp)

target_compile_definitions(cms PRIVATE NEURALA_EXPORT_PLUGIN)
target_include_directories(cms PUBLIC include)
target_link_libraries(cms
    PUBLIC
        stub
    PRIVATE
        external::cms
        CONAN_PKG::cereal)

target_link_libraries(cms_test
    PRIVATE
        external::cms
        CONAN_PKG::cereal)
