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
