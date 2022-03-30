cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(csharp
    SHARED
        src/ResultOutput.cpp
        src/VideoSource.cpp)

add_executable(csharp_test
    src/Main.cpp)

if(WIN32)
    set(STRING_PREFIX "L")

    set_target_properties(csharp
        PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

target_include_directories(csharp
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(csharp_test
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(csharp
    PRIVATE
        stub)

target_link_libraries(csharp_test
    PRIVATE
        ${CMAKE_DL_LIBS}
        stub
        cppcli
        csharp)
