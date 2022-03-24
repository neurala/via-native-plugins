cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(csharp EXCLUDE_FROM_ALL
    SHARED
        src/ResultOutput.cpp
        src/VideoSource.cpp)

add_executable(csharp_test EXCLUDE_FROM_ALL
    src/Main.cpp)

set(NEURALA_DOTNET_PATH "" CACHE PATH "The path to a directory containing the .NET Core headers and libraries.")
set(HOSTFXR_LIBRARY_PATH "" CACHE PATH "The path to the hostfxr library.")

if("${NEURALA_DOTNET_PATH}" STREQUAL "")
    message(SEND_ERROR "NEURALA_DOTNET_PATH not defined for C# sample plugin!")
endif()

add_library(nethost SHARED IMPORTED)
add_library(hostfxr SHARED IMPORTED)

if(WIN32)
    set(NETHOST_LIBRARY "nethost.dll")
elseif(APPLE)
    set(NETHOST_LIBRARY "libnethost.dylib")
else()
    set(NETHOST_LIBRARY "libnethost.so")
endif()

set(NETHOST_LIBRARY_PATH "${NEURALA_DOTNET_PATH}/${NETHOST_LIBRARY}")

target_compile_definitions(csharp_test
    PRIVATE
        "NETHOST_LIBRARY_PATH=\"${NETHOST_LIBRARY_PATH}\""
        "HOSTFXR_LIBRARY_PATH=\"${HOSTFXR_LIBRARY_PATH}\""
        "RESULT_OUTPUT_DLL=\"${RESULT_OUTPUT_DLL}\""
        "VIDEO_SOURCE_DLL=\"${VIDEO_SOURCE_DLL}\""
        "RESULT_OUTPUT_CONFIGURATION=\"${RESULT_OUTPUT_CONFIGURATION}\""
        "VIDEO_SOURCE_CONFIGURATION=\"${VIDEO_SOURCE_CONFIGURATION}\"")

set_target_properties(nethost
    PROPERTIES
        IMPORTED_LOCATION "${NETHOST_LIBRARY_PATH}")

set_target_properties(hostfxr
    PROPERTIES
        IMPORTED_LOCATION "${NEURALA_HOSTFXR}")

target_include_directories(csharp
    PRIVATE
        "${NEURALA_DOTNET_PATH}"
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(csharp_test
    PRIVATE
        "${NEURALA_DOTNET_PATH}"
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

find_package(Threads REQUIRED)

target_link_libraries(csharp
    PRIVATE
        stub
        csharp_assemblies)

target_link_libraries(csharp_test
    PRIVATE
        ${CMAKE_DL_LIBS}
        Threads::Threads
        stub
        csharp_assemblies
        csharp)
