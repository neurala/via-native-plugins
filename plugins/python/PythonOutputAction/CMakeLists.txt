add_library(PythonOutputAction SHARED PythonOutputAction.cpp)

target_include_directories(PythonOutputAction
	PUBLIC
		include
	PRIVATE
		${Python3_INCLUDE_DIRS}
		${B4B_INCLUDE_DIRS}
)

target_compile_definitions(PythonOutputAction
	PRIVATE
		NEURALA_EXPORT_PLUGIN
)

target_compile_options(PythonOutputAction PRIVATE -Wno-write-strings)

target_link_libraries(PythonOutputAction
	PRIVATE
		${_PYTHON3_LIB}
		stub
)

