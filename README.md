# VIA Native Plugins

## Building

### Prerequisites
- [CMake](https://cmake.org/download/#latest) is required to configure the sample plugins and the stub library.
- Conan < 2.0 is used to download external dependencies for some of the plugin samples (such as WebSocket). **Conan 2 is currently not supported, use the latest 1.xx.**
  - On Windows, either use [the installer](https://github.com/conan-io/conan/releases/latest/download/conan-win-64.exe) or install using `pip install conan==1.xx` (where `xx` is the latest available version of Conan 1).
  - On Linux, either install [a package](https://github.com/conan-io/conan/releases/download/1.64.1/conan-ubuntu-64.deb) (this one is specific to Ubuntu) or install using `pip` as shown above.
- The compilation environment should be using the latest [Visual Studio 2019 Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019) (if using Windows) or GCC 10 (if using Linux).

### Usage (Visual Studio)
Visual Studio [supports CMake projects by default](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-160), you should just have to open the directory as a new project, and configure it as you see fit.

### Usage (command line)
From a developer shell (Windows) or a regular terminal (Linux), navigate to this project directory then type:

```
cmake -S"." -B"build" -GNinja
cmake --build build
```

> Note: `Ninja` is only one of the possible CMake generator options. It is installed along with CMake tool by the Visual Studio installer. We recommend it, but you may choose to use
  The Visual Studio generator instead (based on MSBuild, run `cmake --help` for a full list).

## Testing
Basic functional tests in the example plugins are implemented using Boost.Test. The test executables can be run using `build\bin\<plugin>_tests.exe` (Windows) or `build/bin/<plugin>_tests` (Linux).
All generated DLLs and executables (such as the SDK stub library and plugins) are generated in the same directory.

## Writing a plugin

Examples are available for your convenience in the `plugins` directory. The `dummy` plugin may be used as a mock objects for tests
the `empty` plugin contains all the boiler-plate code for a new plugin. The `websocket` plugin is a non-trivial example of a `http`
communication plugin.

To implement your own plugin, the following steps are recommended:

1. Copy the `plugins/empty` plugin in a new folder inside `plugins`
2. In `plugins/CMakeLists.txt` add a new entry for your directory
   > If on Unix, where it is common to have the `lib` prefix, disable it with `set_target_properties(your_library PROPERTIES PREFIX "")`
3. Rename the targets and files in your new directory
4. Implement your business logic. We recommend adding your source files in the `src` folder, then edit the `CMakeLists.txt`
   to add the new files to the build system. Header files can go in the `include` directory, which is added to the compiler
   include directories path.
5. Test your plugin by adding the appropriate files in the `test` directory then adding the source files in the `CMakeLists.txt`.
6. Deploy your plugin by copying the newly compiled DLL in your VIA installation custom plugin folder, which you should specify with the envirionment variable `NEURALA_EXTRA_PLUGINS_PATH`.
  - On Windows, this defaults to `C:\ProgramData\Neurala\SDKService\Plugins`.
  - On Linux there is no default location for extra plugins (plugins are still loaded at SDKService's own library path at `/opt/Neurala/SDKService/lib`).

> If your plugin is not detected by Inspector, restart the Neurala SDKService from Windows' Services panel.

More information on the interfaces can be found in the headers of the stub library. The interface headers are:
- [`VideoSource`](https://github.com/neurala/via-native-plugins/blob/main/stub/include/neurala/video/VideoSource.h)
- [`ResultsOutput`](https://github.com/neurala/via-native-plugins/blob/main/stub/include/neurala/utils/ResultsOutput.h)

---
:warning:
Plugins have to be implemented in C++ using C++17 flags and need to be deployed according to CMake's Release build configuration type.
**Any other parameters flags may result in ABI incompatibility**, and prevent the plugins from being loaded correctly.

---
## FAQ

### What interfaces have to be implemented?
- `CameraDiscoverer` and `VideoSource` for "Input" plugins
- `ResultsOutput` for "Output" plugins

### What is the role of each interface types?
- `CameraDiscoverer` enables the retrieval of information regarding existing cameras or input sources. Each `CameraInfo` is constructed using four parameters:
	1. unique ID (for the SDK to differentiate different cameras connected to the same plugin)
	2. type (name used inside `initMe()` to register the associated `VideoSource` implementation)
	3. name (as displayed in the Inspector GUI)
	4. connection (e.g. IP address, directory path or any string that can be used internally by the plugin)
- `VideoSource` is used to retrieve image metadata and frames. In the call to `create()`, there are two `PluginArguments`, representing the `CameraInfo` retrieved through `CameraDiscoverer` and `Options` object.
- `ResultsOutput` passes back the results of processing each frame as a string in JSON format and an `ImageView` to the associated input.

### How are these types identified and loaded by the Neurala SDK?
The DLL will be scanned for an `extern "C" PLUGIN_API NeuralaPluginExitFunction initMe(NeuralaPluginManager*, std::error_code*)`. Types derived from the three interfaces mentioned above must be registered through the `PluginManager` (`PluginRegistrar`).

Each implementation must define two static functions through which instance lifetime is managed:
- `void* create(PluginArguments&, PluginErrorCallback&)` provides necessary arguments and a function to call in case of errors during instantiation.
- `void destroy(void*)` specifies a pointer to a previously created object that must be deallocated.

### Why is the `ImageMetadata` retrieved separately from `metadata()` and as a part of `ImageView` per each `frame()`?
This is intentional. The call to `metadata()` should return the expected information for the corresponding camera, while the metadata provided as part of each image allows for potential flexibility on a per frame basis.

### Why are there two `frame()` functions?
- `ImageView frame()` must return a pointer to a buffer that remains managed by the plugin until the next call to `nextFrame()`.
- `ImageView frame(std::byte*, std::size_t)` specifies the address to which frame data must be copied and the capacity of the memory block expressed in bytes. Lifetime is afterwards handled by the SDK.

### What is the `stub` library ? Why do I need to link against it ?

The stub library in `/stub` is automatically generated from the current production libraries to provide the subset of symbols required to build a plugin, link and test it without having a complete VIA installation during development.

When installing the final plugin DLL in your VIA installation, the plugin will then dynamically link against the full SDK service library in order to communicate with the SDK service backend.
