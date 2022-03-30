add_custom_command(OUTPUT Managed.dll
    COMMAND dotnet build "${CMAKE_CURRENT_SOURCE_DIR}/Managed" -c Release)

add_library(csharp_assemblies INTERFACE Managed.dll)

set(MANAGED_PLUGIN_DLL "${CMAKE_CURRENT_SOURCE_DIR}/Managed/bin/x64/Release/net472/Managed.dll")

add_subdirectory(CLI)
add_subdirectory(Native)
