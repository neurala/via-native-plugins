cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_executable(test Client.cpp)

target_link_libraries(test CONAN_PKG::boost websocket)
