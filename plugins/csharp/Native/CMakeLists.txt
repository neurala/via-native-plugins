cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(dotnetCamera
    SHARED
        src/Plugin.cpp
        src/ResultOutput.cpp
        src/VideoSource.cpp)

target_compile_definitions(dotnetCamera PRIVATE NEURALA_EXPORT_PLUGIN)

add_executable(dotnetCameraTest
    src/Main.cpp)

if(WIN32)
    set_target_properties(dotnetCamera
        PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

target_include_directories(dotnetCamera
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../CLI")

target_link_libraries(dotnetCamera
    PUBLIC
        cppcli
        stub)

target_link_libraries(dotnetCameraTest
    PRIVATE
        ${CMAKE_DL_LIBS}
        dotnetCamera)
