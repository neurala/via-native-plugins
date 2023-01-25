cmake_minimum_required(VERSION 3.17.0)

# marks debug dlls
set(CMAKE_DEBUG_POSTFIX _d)

add_subdirectory(cms)
add_subdirectory(empty)
add_subdirectory(dummy)
add_subdirectory(websocket)
add_subdirectory(gstreamer)
