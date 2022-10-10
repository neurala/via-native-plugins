cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

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
        external::cms)

target_link_libraries(cms_test
    PRIVATE
        external::cms)
