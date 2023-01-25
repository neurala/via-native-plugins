cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(gstreamer SHARED src/GStreamerVideoSource.cpp)
target_include_directories(gstreamer PUBLIC include)
