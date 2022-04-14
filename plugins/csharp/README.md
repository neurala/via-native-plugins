# .NET Plugin

## Building

Same as building from the root of this repository, but add `--target dotnetCamera` if you want just the plugin DLL files.
Build in Release Mode with `--config Release` for consistently portable results.

## Usage

Put the resulting `dotnetCamera.dll` file in the Neurala Plugins directory (represented by the environment variable `NEURALA_EXTRA_PLUGINS_PATH`, which defaults to `C:\ProgramData\Neurala\SDKService\Plugins`), and put `cppcli.dll` and `Managed.dll` in the SDKService installation directory (`C:\Program Files\Neurala\SDKService\bin`).

## Extending

The provided sample plugin reads image files from `C:\Images`. To extend the plugin for your use cases, replace the sample implementation in `FrameGrabber.cs` with the template below, and put your frame grabbing logic in the `GrabFrame()` method. Each time a frame is required, `GrabFrame()` will be called.

```csharp
public class FrameGrabber {
    public Bitmap GrabFrame() {
        // YOUR CODE GOES HERE
    }
}
```

Here is an example of a possible implementation for a frame grabber that repeatedly reads from an already-populated array of images.

```csharp
public class FrameGrabber {
    private Bitmap[] ImageArray;
    private Int32 CurrentIndex;

    public FrameGrabber() {
        ImageArray = SomehowLoadSomeImages();
        CurrentIndex = 0;
    }

    public Bitmap GrabFrame() {
        var image = ImageArray[CurrentIndex];

        CurrentIndex = (CurrentIndex + 1) % ImageArray.Length;

        return image;
    }
}
```

Here is an example of a possible implementation for a frame grabber that 
