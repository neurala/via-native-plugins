# Result Metadata

## Common Fields

- `brainIndex` - The index of the brain that generated the result.
- `brain` - The "friendly" name of the brain the generated the result.
- `camera` - The "friendly" name of the camera that captured the frame on which inference occurred.
- `framesSkipped` - The number of frames that were skipped.
- `cameraFrameRates` - An map of pipeline indices to FPS values, each corresponding to the rate at which a camera is providing frames to the corresponding pipeline.
- `inferenceFrameRates` - An array of pipeline indices to FPS values, each corresponding to the rate at which a pipeline is generating inference results.

### Example

```json
{
    "brainIndex" : 0,
    "brain" : "My Big Brain",
    "camera" : "USB Camera 5",
    "framesSkipped" : 0,
    "cameraFrameRates" : {
        "0" : 59.0
    },
    "inferenceFrameRates" : {
        "0" : 9.4
    }
}
```

## Classification

In addition to the common fields above, inspection logs specific to Classification have the following additional properties.

- `inferenceResult` - The inference result.
  - `image` - The image for which the results are generated.
  - `results` - An array of inference results.
    - `probability` - The probability level for the result (effectively the confidence divided by the norm).
    - `confidence` - The confidence level for the result.
    - `label` - The class label that was inferred.

### Example

```json
{
    "brainIndex" : 0,
    "brain" : "Art Connoisseur",
    "camera" : "USB Camera 5",
    "framesSkipped" : 0,
    "cameraFrameRates" : {
        "0" : 59.0
    },
    "inferenceFrameRates" : {
        "0" : 9.4
    },
    "inferenceResult" : {
        "image" : "images/paintings/ugly/Marcelle Aron.png",
        "results" : [
            {
                "probability" : 1.0,
                "confidence" : 0.625,
                "label" : "ugly"
            }
        ]
    }
}
```

## Explainability

In addition to the fields for Classification above, results with Explainability enabled have the following addition properties.

- `anomalyMap` - A binary-encoded blob that represents the anomaly map for the result.
- `anomalyOverlay` - A `base64`-encoded string that represents the anomaly overlay (heatmap) for the result.

### Example

```json
{
    "brainIndex" : 0,
    "brain" : "Art Connoisseur",
    "camera" : "USB Camera 5",
    "framesSkipped" : 0,
    "cameraFrameRates" : {
        "0" : 59.0
    },
    "inferenceFrameRates" : {
        "0" : 9.4
    },
    "inferenceResult" : {
        "image" : "images/paintings/ai/random-garbage.png",
        "results" : [
            {
                "probability" : 1.0,
                "confidence" : 0.625,
                "label" : "Nothing I know"
            }
        ]
    },
    "anomalyMap" : "...",
    "anomalyOverlay" : "..."
}
```

## Detection

Results for this type of inspection are similar to those for Classification, except in the results.

- `inferenceResult`
  - `results`
    - `x1`, `y1`, `x2`, `y2` - The coordinates (upper left and lower right) of the bounding box of the result.
    - `id` - The class ID.
    - `confidence` - The confidence level for the result.

### Example

```json
{
    "brainIndex" : 0,
    "brain" : "Art Connoisseur",
    "camera" : "USB Camera 5",
    "framesSkipped" : 0,
    "cameraFrameRates" : {
        "0" : 59.0
    },
    "inferenceFrameRates" : {
        "0" : 9.4
    },
    "inferenceResult" : {
        "image" : "images/paintings/ai/skittles.png",
        "results" : [
            {
                "x1": 0.125,
                "y1": 0.125,
                "x2": 0.25,
                "y2": 0.25,
                "id": "green",
                "confidence": 0.625,
            }
        ]
    },
    "anomalyMap" : "...",
    "anomalyOverlay" : "..."
}
```
