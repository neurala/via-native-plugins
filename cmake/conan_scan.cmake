execute_process(COMMAND conan --version
    OUTPUT_VARIABLE conan_version_str
    OUTPUT_STRIP_TRAILING_WHITESPACE 
    COMMAND_ERROR_IS_FATAL LAST)
    
string(REGEX REPLACE "^Conan version ([0-9]+)\\.([0-9]+)\\.([0-9]+)$"
                     "\\1.\\2.\\3" CONAN_VERSION  "${conan_version_str}")

if (CONAN_VERSION VERSION_GREATER_EQUAL "2.0.0")
    message(FATAL_ERROR "Conan version too high (${CONAN_VERSION}, please install latest 1.xx version)")
endif()

execute_process(COMMAND conan remote list --raw
    OUTPUT_VARIABLE conan_remotes_list
    OUTPUT_STRIP_TRAILING_WHITESPACE 
    COMMAND_ERROR_IS_FATAL LAST)

string(REGEX REPLACE ".*(conan-?center) https://center.conan.io (True|False).*"
                     "\\1" CONAN_CENTER "${conan_remotes_list}")

message(STATUS "Found Conan ${CONAN_VERSION}, using remote: ${CONAN_CENTER}")