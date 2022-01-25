# VIA WebSocket Plugin

## Summary

This plugin provides a way to communicate with extensions to VIA that either do not run on the same machine, or are implemented in a different environment (e.g. Java, .NET, Python, etc). The plugin communicates with a given WebSocket server by requesting image data and sending inspection results, effectively acting as both a camera (`VideoSource`) and a sink for results (`ResultsOutput`).

## Building

This plugin will be built with the rest in this repository. See the root-level `README.md`.

## Environment Variables

- `NEURALA_SERVER_IP_ADDRESS`
  - The IP address of the WebSocket server to which this plugin should connect.
- `NEURALA_SERVER_PORT`
  - The port to use when connecting.

## Protocol

### Metadata Request (Plugin → Server)

```json
{
  "request": "metadata"
}
```

### Metadata Response (Plugin ← Server)

```json
{
  "width": "(image width in pixels)",
  "height": "(image height in pixels)",
  "layout": "planar|interleaved|semiplanar", // Usually "interleaved"
  "dataType": "boolean|uint8|uint16|binary16|binary32|binary64", // Usually "uint8"
  "colorSpace": "grayscale|RGB|RGBA|BGR|BGRA|RGB565|HSV|bayerRG|bayerGR|bayerBG|bayerGB|YUV420|NV12|NV21|YUV422",
  "orientation": "topLeft|topRight|bottomRight|bottomLeft|leftTop|rightTop|rightBottom|leftBottom" // Usually "topLeft"
}
```

### Frame Request (Plugin → Server)

```json
{
  "request": "frame"
}
```

### Frame Response (Plugin ← Server)

Instead of responding with a JSON, the server should respond with the raw pixel data of the current frame. For example, if the images were in interleaved `uint8` BGR format, the response would resemble the following, where each triplet of letters represents one pixel containing a blue value, a green value, and a red value.

```
BGRBGRBGRBGRBGR...
```

### Result Request (Plugin → Server)

```json
{
  "request": "result",
  "body":
  {
    // see ResultsOutput::operator()
  }
}
```

### Result Response (Plugin ← Server)

```json
{}
```

### Execute Request (Plugin → Server)

```json
{
  "request": "execute",
  "body":
  {
    "action": "(see VideoSource::execute)"
  }
}
```

### Execute Response (Plugin ← Server)

```json
{}
```
