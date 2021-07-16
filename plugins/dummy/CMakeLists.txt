cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(dummy SHARED src/DummyVideoPlugin.cpp)

target_include_directories(dummy PUBLIC include)
	
target_include_directories(dummy SYSTEM PRIVATE
	../include
	../build/msvc-release/include)

target_compile_options(dummy PRIVATE /permissive- /Zc:preprocessor /Zc:__cplusplus /MP /wd4251)

target_link_libraries(dummy stub)