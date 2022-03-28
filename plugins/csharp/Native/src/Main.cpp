#include <cassert>
#include <cstdio>
#include <thread>
#include <type_traits>

#include <coreclr_delegates.h>
#include <hostfxr.h>
#include <nethost.h>

#ifdef _WIN32
#include <windows.h>
using cstring = const char_t*;
#define STR(string) L ## string
#else
#include <dlfcn.h>
#include <limits.h>
#define MAX_PATH PATH_MAX
using cstring = const char*;
#define STR(string) string
#endif

#include "ResultOutput.h"
#include "VideoSource.h"

namespace
{

auto
loadLibrary(const char* path)
{
#ifdef _WIN32
	return LoadLibraryA(path);
#else
	return dlopen(path, RTLD_LAZY | RTLD_LOCAL);
#endif
}

template<class T>
auto
getSymbolAddress(void* handle, const char* name)
{
#ifdef _WIN32
	return reinterpret_cast<T>(GetProcAddress(reinterpret_cast<HMODULE>(handle), name));
#else
	return reinterpret_cast<T>(dlsym(handle, name));
#endif
}

class hostfxr
{
private:
	hostfxr_initialize_for_runtime_config_fn initializer;
	hostfxr_get_runtime_delegate_fn runtimeDelegate;
	hostfxr_close_fn closer;

public:
	explicit hostfxr(void* nethost)
	: initializer(getSymbolAddress<hostfxr_initialize_for_runtime_config_fn>(nethost, "hostfxr_initialize_for_runtime_config")),
	  runtimeDelegate(getSymbolAddress<hostfxr_get_runtime_delegate_fn>(nethost, "hostfxr_get_runtime_delegate")),
	  closer(getSymbolAddress<hostfxr_close_fn>(nethost, "hostfxr_close"))
	{
		assert(initializer && runtimeDelegate && closer);
	}

	auto
	getLoadAssembly(cstring path) const
	{
		void* assemblyLoader;
		hostfxr_handle handle;

		auto status = initializer(path, nullptr, &handle);

		printf("%08X\n", status);

		assert(status == 0 && handle);

		status = runtimeDelegate(handle, hdt_load_assembly_and_get_function_pointer, &assemblyLoader);

		printf("%08X\n", status);

		assert(status == 0 && assemblyLoader);

		closer(handle);

		return reinterpret_cast<load_assembly_and_get_function_pointer_fn>(assemblyLoader);
	}

	auto
	loadMethod(load_assembly_and_get_function_pointer_fn loader, cstring dll, cstring type, cstring method) const
	{
		printf("Loading method %s from DLL %s...", method, dll);
		void* delegate;
		const auto status = loader(dll, type, method, nullptr, nullptr, &delegate);
		printf("(%08X)\n", status);
		assert(status == 0);
		return reinterpret_cast<component_entry_point_fn>(delegate);
	}
};

} // namespace

void
dummy(void (*f)())
{
	std::thread t(f);
	t.join();
}

int
main(int, char**)
{
	const auto nethost = loadLibrary(HOSTFXR_LIBRARY_PATH);

	assert(nethost);

	const hostfxr fxr(nethost);

	const auto loadAssembly = fxr.getLoadAssembly(MANAGED_PLUGIN_CONFIGURATION);
	const auto invoker = fxr.loadMethod(loadAssembly, MANAGED_PLUGIN_DLL, STR("Neurala.ResultOutput, Neurala"), STR("Invoke"));
	const auto metdataGetter = fxr.loadMethod(loadAssembly, MANAGED_PLUGIN_DLL, STR("Neurala.VideoSource, Neurala"), STR("GetMetadata"));
	const auto nextFramer = fxr.loadMethod(loadAssembly, MANAGED_PLUGIN_DLL, STR("Neurala.VideoSource, Neurala"), STR("MoveNextFrame"));
	const auto frameGetter = fxr.loadMethod(loadAssembly, MANAGED_PLUGIN_DLL, STR("Neurala.VideoSource, Neurala"), STR("GetFrame"));
	const auto executor = fxr.loadMethod(loadAssembly, MANAGED_PLUGIN_DLL, STR("Neurala.VideoSource, Neurala"), STR("Execute"));

	neurala::CSharpResultOutput resultOutput(invoker);
	neurala::CSharpVideoSource videoSource(metdataGetter, nextFramer, frameGetter, executor);

	return 0;
}
