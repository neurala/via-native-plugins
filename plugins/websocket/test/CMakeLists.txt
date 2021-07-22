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
target_compile_definitions(test PRIVATE NEURALA_IMPORT_PLUGIN)

file(COPY ${CMAKE_BINARY_DIR}/stub/stub.dll
	DESTINATION ${CMAKE_BINARY_DIR}/plugins/websocket/bin)
