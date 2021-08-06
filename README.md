# VIA Native Plugins

## Building
```
mkdir build
cd build
cmake -GNinja ..
ninja
```

## Testing
Tests are implemented using Boost.Test. The test executables can be run using `bin/<plugin>_tests.exe`.
All generated DLLs and executables (such as the SDK and plugins) are generated in the same directory.

## Writing your first plugin (WIP)
- Have a look at the sample plugins in `/plugins`
- The stub library in `/stub` is automatically generated from the current production libraries to provide the subset of symbols required to build a plugin
- The compiler environment should be using the same [Visual Studio 2019 Build Tools](https://docs.microsoft.com/en-us/visualstudio/releases/2019/release-notes) version as the one used by Neurala to build the production libraries (Currently using version 16.XX.XX)

## FAQ
1. What interfaces have to be implemented?
	- `Camera Discoverer`
	- `VideoSource`
	- `ResultsOutput`
2. What is the role of each interface types?
	- `CameraDiscoverer` enables the retrieval of information regarding existing cameras. Each `CameraInfo` is constructed using four parameters:
		1. unique ID (e.g. MAC address for eBUS cameras)
		2. type (name used inside `initMe()` to register the associated `VideoSource` implementation)
		3. name (dispalyed in the UI)
		4. connection (e.g. IP address or directory path)
	- `VideoSource` is used to retrieve image metadata and frames. In the call to `create()`, there are two `PluginArguments`, representing the `CameraInfo` retrieved through `CameraDiscoverer` and `Option` object.
	- `ResultsOutput` passes back the results of processing each frame as a string in JSON format and an `ImageView` to the associated input.
3. How are these types identified and loaded by the Neurala SDK?
	The DLL will be scanned for an `extern "C" PLUGIN_API NeuralaPluginExitFunction initMe(NeuralaPluginManager*, NeuralaPluginStatus*)`. Types derived from the three interfaces mentioned above must be registered through the `PluginManager` (`PluginRegistrar`).
	Each implementation must define two static functions through which instance lifetime is managed:
	- `void* create(PluginArguments&, PluginErrorCallback&)` provides necessary arguments and a function to call in case of errors during instantiation.
	- `void destroy(void*)` specifies a pointer to a previously created object that must be deallocated.
4. Why is the `ImageMetadata` retrieved separately and as a part of `ImageView` per each frame?
	This is intentional. The call to `metadata()` should return the expected information for the corresponding camera, while the metadata provided as part of each image allows for potential flexibility on a per frame basis.
5. Why are there two `frame()` functions?
	- `ImageView frame()` must return a pointer to a buffer that remains managed by the plugin until the next call to `nextFrame()`.
	- `ImageView frame(std::byte*, std::size_t)` specifies the address to which frame data must be copied and the capacity of the memory block expressed in bytes. Lifetime is afterwards handled by the SDK.
