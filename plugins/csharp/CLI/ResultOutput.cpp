#include <msclr/marshal.h>
#include <msclr/marshal_windows.h>

#using MANAGED_PLUGIN_DLL

void invokeResultOutput(const char * metadata, void * imageBytes, int width, int height) {
    Neurala::ResultOutput::Invoke(msclr::interop::marshal_as<System::String^>(metadata), msclr::interop::marshal_as<System::IntPtr>(imageBytes), width, height);
}
