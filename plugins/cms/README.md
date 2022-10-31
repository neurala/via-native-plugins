# VIA SILIOS Plugin

## Summary

This plugin provides a way to communicate with a SILIOS-compatible camera. Currently, only the Toucan T4 is supported
and known to work with this plugin, but other SILIOS-supported cameras may work if there are corresponding XML files
available. Due to a limitation in the CMS driver this plugin uses, which causes FPS and gain to be reset each time the
camera is connected, a Neurala-specific overrides file `NeuralaCMSOverrides.json` may be provided as a workaround to
control those two parameters before VIA uses the camera. All of the other parameters, such as black level, exposure
time, pixel clock, etc may be controlled by external means as usual. An example is provided below.

    ```json
    {
        "frameRate" : 10.0,
        "gain" : 31.0
    }
    ```

## Building

This plugin depends on the CMS library provided by SILIOS. The CMS library has a fixed set of libraries and headers on
which it depends. To avoid breakage, those libraries have been packaged verbatim and uploaded to Dropbox. In order for
this plugin to function, those files must be unzipped to the [`external`](external) directory prior to running CMake.
Because of this quirk, it will not be built alongside the other plugins of this repository. To build, simply select the
`cms` target (i.e. `--target cms`) when building from the top level of this repository.
