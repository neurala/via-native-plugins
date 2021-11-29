# VIA C# Plugin

## Summary

This is a minimal .NET interface for VIA native plugins, such that plugins can be implemented in a .NET language environment such as C#, F#, or Visual Basic. This library provides a WebSocket server that communicates with the native WebSocket plugin for providing image frames to it and retrieving results from it.

## Building

For convenience, the plugin and an executable for testing are provided in pre-built form. The library `Binaries/Neurala.VIA.WebSocket.dll` can be directly referenced in any existing project. You can also simply add `Sources/Server.cs` directly into your project and reference `Binaries/Newtonsoft.Json.dll` and `Binaries/websocket-sharp.dll` (or your own compatible versions thereof). Otherwise, the plugin can be built via `dotnet` or by loading `VIAWS.sln` into Visual Studio.

- `dotnet build VIAWS.sln` to build everything
  - `dotnet build VIAWS` for the library itself
  - `dotnet build Test` for the sample program (see below for usage)

## Synopsis

```csharp
namespace Neurala.VIA {
    public interface IRequestHandler {
        /// <summary>Handles the results of an inspection (see <c>ResultsOutput::operator()</c>).</summary>
        void HandleResults(string results);

        /// <summary>Executes a given action (see <c>VideoSource::execute</c>).</summary>
        void ExecuteAction(string action);
    }

    public class WebSocket {
        /// <summary>Constructs a VIA WebSocket.</summary>
        /// <param name="port">The port on which the server should listen.</param>
        /// <param name="bitmapProducer">A user-provided source of image frames.</param>
        /// <param name="requestHandler">An interface for handling actions and inspection results.</param>
        public WebSocket(int port, IEnumerable<Bitmap> bitmapProducer, IRequestHandler requestHandler);

        /// <summary>Starts the server. This method does not block.</summary>
        public void Start();

        /// <summary>Stops the server.</summary>
        public void Stop();
    }
}
```

## Example Usage

```csharp
public static class Program {
    public static void Main() {
        var port = 1234;
        var images = GetImages();
        var handler = new MyHandler();
        var server = new Neurala.VIA.WebServer(port, images, handler);

        server.Start();
        /* ... */
        server.Stop();
    }

    private static IEnumerable<Bitmap> GetImages() {
        var images = Directory.EnumerateFiles("/images/dogs");

        foreach (var image in images)
            yield return new Bitmap(image);
    }
}

class MyHandler : IRequestHandler {
    void HandleResults(string results) {
        // Handle inspection results.
        // See ResultsOutput for the results format.
    }

    void ExecuteAction(string action) {
        // Execute a user-defined action.
    }
}
```

A test application like the above sample is currently provided in the `Binaries` directory. To use it, simply run it as in the following example before starting inspections with the native WebSocket plugin.

```bash
Binaries/Test.exe $PORT $IMAGE_DIRECTORY
```
