cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(dummy SHARED src/DummyVideoPlugin.cpp)

target_include_directories(dummy PUBLIC include)

target_link_libraries(dummy stub)
