cmake_minimum_required(VERSION 3.17.0)

project(stream)

set(CMAKE_CXX_STANDARD 17)

include(conan.cmake)

conan_cmake_run(
    REQUIRES boost/1.76.0
    BASIC_SETUP CMAKE_TARGETS
    BUILD_TYPE ${CMAKE_BUILD_TYPE}
    BUILD missing
    OUTPUT_QUIET
    KEEP_RPATHS)

add_executable(stream src/main.cpp)

target_link_libraries(stream PUBLIC CONAN_PKG::boost)

target_include_directories(
    stream
    PUBLIC
    include
    PRIVATE
    ../../include
    ../../build/include
    ../../external/jsoncpp/include
    ../../../../include
    ../../../../build/include)
