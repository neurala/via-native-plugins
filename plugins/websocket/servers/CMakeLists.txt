cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_executable(StandaloneServer src/Server.cpp src/IOServer.cpp src/StandaloneServer.cpp)
target_include_directories(StandaloneServer PUBLIC include)
target_link_libraries(StandaloneServer PUBLIC stub CONAN_PKG::boost)
