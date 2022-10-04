#include <msclr/marshal.h>
#include <msclr/marshal_windows.h>

#include "Bridge.h"

#using MANAGED_PLUGIN_DLL

namespace neurala::dotnet {
    void initialize() {
        Neurala::Program::Main();
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
