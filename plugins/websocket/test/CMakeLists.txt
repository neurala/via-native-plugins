cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_executable(websocket_tests
	Client.cpp
	Discoverer.cpp
	FullSequence.cpp
	Init.cpp
	Input.cpp
	Output.cpp
	../servers/src/Server.cpp
	../servers/src/IOServer.cpp)
target_include_directories(websocket_tests PRIVATE ../servers/include)
target_link_libraries(websocket_tests CONAN_PKG::boost websocket)
