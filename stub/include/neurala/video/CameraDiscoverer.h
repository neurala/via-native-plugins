/*
 * Copyright (c) Neurala Inc. 2013-2021
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:  The above copyright notice and this
 * permission notice (including the next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef NEURALA_VIDEO_CAMERA_DISCOVERER_H
#define NEURALA_VIDEO_CAMERA_DISCOVERER_H

#include <vector>

#include "neurala/video/dto/CameraInfo.h"

namespace neurala
{
/**
 * @brief Base class for a video data source discoverer. Plugin implementers should inherit from
 * this type.
 */
class CameraDiscoverer
{
public:
	virtual ~CameraDiscoverer() = default;

	// Scan for all available cameras
	[[nodiscard]] virtual std::vector<dto::CameraInfo> operator()() const noexcept = 0;
};

} // namespace neurala

#endif // NEURALA_VIDEO_CAMERA_DISCOVERER_H
