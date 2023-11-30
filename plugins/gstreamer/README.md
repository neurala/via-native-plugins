# GStreamer plugin

## Building

Requires the following dependencies:

```
gtk3 
gstreamer
gstreamer-sdp
gstreamer-app
gstreamer-video
```

For building and deployment, see the [top level README](../../README.md).

## Usage

The GStreamer plugin launches a user-defined pipeline and exposes it as a camera to Neurala Inspector.

Users should define their pipeline in an envrionment variable:

```
export NEURALA_GSTREAMER_PIPELINE=videotestsrc ! video/x-raw,format=RGB,width=320,height=240 ! appsink --name=neurala_appsink
```

The plugin is looking for the element named "neurala_appsink" to use as its own source of image. It is recommended to define the stream images size in the pipeline to avoid feeding images that are larger than necessary.

The Neurala infernce engine must be restarted eveytime a change is make to this pipeline:

```
sudo systemctl restart neurala_sdkservice
```