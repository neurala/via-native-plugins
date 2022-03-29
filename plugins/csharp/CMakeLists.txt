add_custom_command(OUTPUT Managed.dll
    COMMAND dotnet build "${CMAKE_CURRENT_SOURCE_DIR}/Managed" --no-self-contained -c Release)

add_library(csharp_assemblies INTERFACE Managed.dll)

set(MANAGED_PLUGIN_DLL "${CMAKE_CURRENT_SOURCE_DIR}/Managed/bin/Release/net5.0/Managed.dll")
set(MANAGED_PLUGIN_CONFIGURATION "${CMAKE_CURRENT_SOURCE_DIR}/Managed/bin/Release/net5.0/Managed.runtimeconfig.json")

add_subdirectory(Native)
