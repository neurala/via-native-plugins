cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(plugin_headers INTERFACE)

target_include_directories(plugin_headers INTERFACE ${CMAKE_CURRENT_LIST_DIR})

add_library(neurala::plugin_headers ALIAS plugin_headers)

install(
	DIRECTORY
		${CMAKE_CURRENT_LIST_DIR}/neurala
	DESTINATION
		${INCLUDE_INSTALL_DIR}
	COMPONENT
		devel
	FILES_MATCHING
		PATTERN "*.h")
