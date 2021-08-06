# VIA Native Plugins

## Building

### Prerequisites :
- [Conan](https://github.com/conan-io/conan/releases/latest/download/conan-win-64.exe) is used to download external dependencies for the plugin samples
- The compilation environment should be using the latest [Visual Studio 2019 Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019)

### Usage :
```
mkdir build
cd build
cmake -GNinja ..
ninja
```

## Testing
Basic functional tests in the example plugins are implemented using Boost.Test. The test executables can be run using `bin/<plugin>_tests.exe`.
All generated DLLs and executables (such as the SDK stub library and plugins) are generated in the same directory.

## Writing your first plugin
- Have a look at the sample plugins in `/plugins`
- Implement the interfaces that your plugin requires and build your project as a shared library (DLL)
- Write simple tests to make sure that the plugin can be loaded and that you are able to send image data or receive prediction data through the API
- Install your plugin DLL in a VIA installation for real world testing
- Along with the sample plugins, it is highly recommended to read the documentation available in the headers for the [`VideoSource`](https://github.com/neurala/via-native-plugins/blob/main/stub/include/neurala/video/VideoSource.h) and [`ResultsOutput`](https://github.com/neurala/via-native-plugins/blob/main/stub/include/neurala/utils/ResultsOutput.h) interfaces
## FAQ
1. What interfaces have to be implemented?
	- `CameraDiscoverer` and `VideoSource` for "Input" plugins
	- `ResultsOutput` for "Output" plugins

2. What is the role of each interface types?
	- `CameraDiscoverer` enables the retrieval of information regarding existing cameras or input sources. Each `CameraInfo` is constructed using four parameters:
		1. unique ID (for the SDK to differentiate different cameras connected to the same plugin)
		2. type (name used inside `initMe()` to register the associated `VideoSource` implementation)
		3. name (as displayed in the Inspector GUI)
		4. connection (e.g. IP address, directory path or any string that can be used internally by the plugin)
	- `VideoSource` is used to retrieve image metadata and frames. In the call to `create()`, there are two `PluginArguments`, representing the `CameraInfo` retrieved through `CameraDiscoverer` and `Option` object.
	- `ResultsOutput` passes back the results of processing each frame as a string in JSON format and an `ImageView` to the associated input.

3. How are these types identified and loaded by the Neurala SDK?
	
	The DLL will be scanned for an `extern "C" PLUGIN_API NeuralaPluginExitFunction initMe(NeuralaPluginManager*, NeuralaPluginStatus*)`. Types derived from the three interfaces mentioned above must be registered through the `PluginManager` (`PluginRegistrar`).
	Each implementation must define two static functions through which instance lifetime is managed:
	- `void* create(PluginArguments&, PluginErrorCallback&)` provides necessary arguments and a function to call in case of errors during instantiation.
	- `void destroy(void*)` specifies a pointer to a previously created object that must be deallocated.

4. Why is the `ImageMetadata` retrieved separately from `metadata()` and as a part of `ImageView` per each `frame()`?

	This is intentional. The call to `metadata()` should return the expected information for the corresponding camera, while the metadata provided as part of each image allows for potential flexibility on a per frame basis.

5. Why are there two `frame()` functions?
	- `ImageView frame()` must return a pointer to a buffer that remains managed by the plugin until the next call to `nextFrame()`.
	- `ImageView frame(std::byte*, std::size_t)` specifies the address to which frame data must be copied and the capacity of the memory block expressed in bytes. Lifetime is afterwards handled by the SDK.

6. What is the `stub` library ? Why do I need to link against it ?

	The stub library in `/stub` is automatically generated from the current production libraries to provide the subset of symbols required to build a plugin, link and test it without having a complete VIA installation during development.

	When installing the final plugin DLL in your VIA installation, the plugin will then dynamically link against the full SDK service library in order to communicate with the SDK service backend.

