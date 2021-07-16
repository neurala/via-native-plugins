cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_executable(empty src/InitMe.cpp src/main.cpp)

target_include_directories(empty
    PUBLIC
        include
    PRIVATE
        ../../../../build/include
        ../../../../external/cereal/include
        ../../../../external/jsoncpp/include
        ../../../../include
        ../../../../../../build/include
        ../../../../../../include)
