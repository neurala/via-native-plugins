add_library(cppcli SHARED ResultOutput.cpp)

set_target_properties(cppcli
    PROPERTIES
        COMMON_LANGUAGE_RUNTIME ""
        VS_DOTNET_REFERENCES "System"
        WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_compile_definitions(cppcli
    PRIVATE
        "MANAGED_PLUGIN_DLL=\"${MANAGED_PLUGIN_DLL}\"")

target_link_libraries(cppcli
    PRIVATE
        csharp_assemblies)

add_custom_command(TARGET cppcli POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MANAGED_PLUGIN_DLL}"
        $<TARGET_FILE_DIR:cppcli>)