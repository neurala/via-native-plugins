cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(neuralaVideoPluginGST SHARED src/GStreamerVideoSource.cpp)
target_include_directories(neuralaVideoPluginGST PUBLIC include ../../stub/include ${CMAKE_BINARY_DIR}/include)

find_package(PkgConfig REQUIRED)

pkg_check_modules(gtk3 REQUIRED IMPORTED_TARGET gtk+-3.0)
pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0>=1.4)
pkg_search_module(gstreamer-sdp REQUIRED IMPORTED_TARGET gstreamer-sdp-1.0>=1.4)
pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0>=1.4)
pkg_search_module(gstreamer-video REQUIRED IMPORTED_TARGET gstreamer-video-1.0>=1.4)

target_link_libraries(neuralaVideoPluginGST stub)
target_link_libraries(neuralaVideoPluginGST
    PkgConfig::gtk3
    PkgConfig::gstreamer
    PkgConfig::gstreamer-sdp
    PkgConfig::gstreamer-app
    PkgConfig::gstreamer-video)

add_executable(gstreamer_test src/Test.cpp)
target_link_libraries(gstreamer_test PRIVATE neuralaVideoPluginGST)
