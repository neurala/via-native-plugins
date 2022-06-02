#include <msclr/marshal.h>
#include <msclr/marshal_windows.h>

#include "Bridge.h"

#using MANAGED_PLUGIN_DLL

namespace neurala::dotnet {
    void initialize() {
        const auto executingAssembly = System::Reflection::Assembly::GetExecutingAssembly()->Location;
        const auto containingDirectory = System::IO::Directory::GetParent(executingAssembly)->ToString();
        const auto pluginAssemblyName = System::IO::Path::GetFileName(MANAGED_PLUGIN_DLL);
        const auto pluginAssembly = System::IO::Path::Combine(containingDirectory, pluginAssemblyName);

        System::Reflection::Assembly::LoadFile(pluginAssembly);
    }
}

namespace neurala::dotnet::result_output {
    void invokeResultOutput(const char* metadata, const void* imageBytes, int width, int height) {
        Neurala::ResultOutput::Invoke(msclr::interop::marshal_as<System::String^>(metadata),
                                      msclr::interop::marshal_as<System::IntPtr>(const_cast<void*>(imageBytes)),
                                      width,
                                      height);
    }
}

namespace neurala::dotnet::video_source {
    void getMetadata(int& width, int& height) {
        Neurala::VideoSource::GetMetadata(width, height);
    }

    void moveNextFrame(int& status) {
        Neurala::VideoSource::MoveNextFrame(status);
    }

    void getFrame(void* buffer) {
        Neurala::VideoSource::GetFrame(msclr::interop::marshal_as<System::IntPtr>(buffer));
    }

    void execute(const char* action) {
        Neurala::VideoSource::Execute(msclr::interop::marshal_as<System::String^>(action));
    }
}
