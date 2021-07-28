# VIA Native Plugins

## Building
```
mkdir build
cd build
cmake -GNinja ..
ninja
```

## Testing
Tests are implemented using Boost.Test. The test executable can be ran using `bin/test.exe`.
All generated DLLs and executables (such as the B4B stub and plugins) are generated in the same directory.

## Writing your first plugin (WIP)

- Have a look at the sample plugins in `/plugins`
- The stub library in `/stub` is automatically generated from the current production libraries to provide the subset of symbols required to build a plugin
- The compiler environment should be using the same [Visual Studio 2019 Build Tools](https://docs.microsoft.com/en-us/visualstudio/releases/2019/release-notes) version as the one used by Neurala to build the production libraries (Currently using version 16.XX.XX)
