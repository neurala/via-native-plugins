cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

if(WIN32 AND MSVC)
    add_compile_options(/Zc:__cplusplus)
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

add_executable(websocket src/Client.cpp src/InitMe.cpp src/Input.cpp src/Server.cpp src/main.cpp)

target_link_libraries(websocket PUBLIC CONAN_PKG::boost)

target_include_directories(websocket
    PUBLIC
        include
    PRIVATE
        ../../../../build/include
        ../../../../external/cereal/include
        ../../../../external/jsoncpp/include
        ../../../../include
        ../../../../../../build/include
        ../../../../../../include)

# add_subdirectory(test)
