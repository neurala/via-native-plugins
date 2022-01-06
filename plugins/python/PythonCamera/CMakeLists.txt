add_library(PythonCamera SHARED PythonCamera.cpp)

target_include_directories(PythonCamera
	PUBLIC
		include
	PRIVATE
		${Python3_INCLUDE_DIRS}
		${B4B_INCLUDE_DIRS}
)

#target_compile_definitions(PythonCamera
#	PRIVATE
#		NEURALA_EXPORT_PLUGIN
#)

target_link_libraries(PythonCamera
	PRIVATE
		${_PYTHON3_LIB}
		stub
)

