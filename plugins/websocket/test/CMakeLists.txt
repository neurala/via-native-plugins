cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_executable(test
	Client.cpp
	Discoverer.cpp
	FullSequence.cpp
	Init.cpp
	Input.cpp
	Output.cpp)

target_link_libraries(test CONAN_PKG::boost websocket)

# Flip PLUGIN_API declarations in DLL headers from exports to imports.
target_compile_definitions(test PRIVATE NEURALA_IMPORT_PLUGIN)
