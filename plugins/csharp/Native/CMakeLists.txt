cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_STANDARD 17)

add_library(csharp EXCLUDE_FROM_ALL
    SHARED
        src/ResultOutput.cpp
        src/VideoSource.cpp)

add_executable(csharp_test EXCLUDE_FROM_ALL
    src/Main.cpp)

set(NEURALA_DOTNET_PATH "C:\\Program Files\\dotnet\\packs\\Microsoft.NETCore.App.Host.win-x64\\5.0.15\\runtimes\\win-x64\\native" CACHE PATH "The path to a directory containing the .NET Core headers and libraries.")
set(HOSTFXR_LIBRARY_PATH "C:\\Program Files\\dotnet\\host\\fxr\\5.0.15\\hostfxr.dll" CACHE PATH "The path to the hostfxr library.")

if(WIN32)
    set(STRING_PREFIX "L")
endif()

target_compile_definitions(csharp_test
    PRIVATE
        "HOSTFXR_LIBRARY_PATH=\"${HOSTFXR_LIBRARY_PATH}\""
        "MANAGED_PLUGIN_DLL=${STRING_PREFIX}\"${MANAGED_PLUGIN_DLL}\""
        "MANAGED_PLUGIN_CONFIGURATION=${STRING_PREFIX}\"${MANAGED_PLUGIN_CONFIGURATION}\"")

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
