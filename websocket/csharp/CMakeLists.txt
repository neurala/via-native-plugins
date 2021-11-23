cmake_minimum_required(VERSION 3.8)

project(VIAWS LANGUAGES CSharp)

add_library(Neurala.VIA.WebSocket Sources/Server.cs)

set_target_properties(Neurala.VIA.WebSocket PROPERTIES 
    VS_DOTNET_REFERENCE_Newtonsoft.Json
        "Binaries/Newtonsoft.Json.dll"
    VS_DOTNET_REFERENCE_websocket-sharp
        "Binaries/websocket-sharp.dll")

add_executable(Neurala.VIA.Test Sources/Test.cs)
