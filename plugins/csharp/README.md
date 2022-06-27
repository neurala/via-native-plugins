# .NET Plugin

## Building

Either add the C# project in the `Managed` directory to an existing (or new) solution, or build the DLL directly with the following command.

```
dotnet build Managed -c Release
```

## Usage

Put the resulting `Managed.dll` in the SDKService installation directory (`C:\Program Files\Neurala\SDKService\bin`). There are also two libraries in `Precompiled`. Put `Precompiled/cppcli.dll` in the SDKService installation directory alongside `Managed.dll`, and put `Precompiled/dotnetCamera.dll` in the Neurala plugins directory (`C:\ProgramData\Neurala\SDKService\Plugins`).

## Extending

To extend the plugin for your use cases, replace the sample implementation in `Program.cs` with the template below, and put your frame grabbing logic in the `Main()` method. Whenever you have a frame ready, call `VideoSource.SendImage(...)` with your frame as a `Bitmap` object. Here is an example implementation, which repeatedly reads images from the `C:\Images` directory, sends them for processing, and then reads the result. Please beware that the method signature may not be changed, as it is not an actual program entry point.

```csharp
namespace Neurala {
    public static class Program {
        public static void Main() {
            var imageFiles = Directory.EnumerateFiles(@"C:\Images");

            while (true) {
                foreach (var imageFile in imageFiles) {
                    var bitmap = new Bitmap(imageFile);
                    var result = VideoSource.SendImage(bitmap);

                    Console.WriteLine(result);
                }
            }
        }
    }
}
```

## Integrating

The directory [Managed](Managed) represents the C# portion of this .NET project. For simplicity of the sample, it has no dependencies and is intentionally bare. It can be treated like any other .NET project (including via `dotnet` of .NET Core) and can be integrated into an existing (or new) Visual Studio solution.
