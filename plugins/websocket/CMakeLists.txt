cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_compile_options(/Zc:__cplusplus /permissive- /Zc:preprocessor /Zc:__cplusplus /MP /wd4251 /wd5104 /wd5105)
endif()

include(conan)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CONAN_BUILD_TYPE "Debug")
else()
    set(CONAN_BUILD_TYPE "Release")
endif()

conan_cmake_run(
    REQUIRES boost/1.76.0
    BASIC_SETUP CMAKE_TARGETS
    BUILD_TYPE ${CONAN_BUILD_TYPE}
    BUILD missing
    OUTPUT_QUIET
    KEEP_RPATHS)

add_library(websocket STATIC
    src/Client.cpp
    src/InitMe.cpp
    src/Input.cpp
    src/Server.cpp)

target_include_directories(websocket INTERFACE include PRIVATE include/websocket)

target_link_libraries(websocket PUBLIC stub CONAN_PKG::boost)

add_subdirectory(test)