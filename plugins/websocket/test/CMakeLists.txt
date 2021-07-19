cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

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

add_executable(tests Client.cpp)

target_link_libraries(tests PUBLIC CONAN_PKG::boost)
