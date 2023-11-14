cmake_minimum_required(VERSION 3.17.0)

# marks debug dlls
set(CMAKE_DEBUG_POSTFIX _d)

option(NEURALA_BUILD_PLUGIN_WEBSOCKET "Video Source/Sink for websocket" OFF)
option(NEURALA_BUILD_PLUGIN_CMS "Video Source/Sink for Silios cameras (REQUIRES Silios CMS)" OFF)
option(NEURALA_BUILD_PLUGIN_GSTREAMER "Video Source/Sink for GStreamer" OFF)

# Check Conan version and get Conan center local name
if(NEURALA_BUILD_PLUGIN_WEBSOCKET OR NEURALA_BUILD_PLUGIN_CMS)
    include(conan_scan)
endif()

add_subdirectory(empty)
add_subdirectory(dummy)

if(NEURALA_BUILD_PLUGIN_WEBSOCKET)
    add_subdirectory(websocket)
endif()

if(NEURALA_BUILD_PLUGIN_CMS)
    add_subdirectory(cms)
endif()

if(NEURALA_BUILD_PLUGIN_GSTREAMER)
    add_subdirectory(gstreamer)
endif()
