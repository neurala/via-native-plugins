add_custom_command(OUTPUT ResultOutput.dll
    COMMAND dotnet build "${CMAKE_CURRENT_SOURCE_DIR}/ResultOutput" -c Release)

add_custom_command(OUTPUT VideoSource.dll
    COMMAND dotnet build "${CMAKE_CURRENT_SOURCE_DIR}/VideoSource" -c Release)

add_library(csharp_assemblies INTERFACE ResultOutput.dll VideoSource.dll)

set(RESULT_OUTPUT_DLL "${CMAKE_CURRENT_SOURCE_DIR}/ResultOutput/bin/Release/net5.0/ResultOutput.dll")
set(RESULT_OUTPUT_CONFIGURATION "${CMAKE_CURRENT_SOURCE_DIR}/ResultOutput/bin/Release/net5.0/ResultOutput.runtimeconfig.json")

set(VIDEO_SOURCE_DLL "${CMAKE_CURRENT_SOURCE_DIR}/VideoSource/bin/Release/net5.0/VideoSource.dll")
set(VIDEO_SOURCE_CONFIGURATION "${CMAKE_CURRENT_SOURCE_DIR}/VideoSource/bin/Release/net5.0/VideoSource.runtimeconfig.json")

add_subdirectory(Native)
