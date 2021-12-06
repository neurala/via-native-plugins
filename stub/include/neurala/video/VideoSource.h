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

#ifndef NEURALA_VIDEO_VIDEO_SOURCE_H
#define NEURALA_VIDEO_VIDEO_SOURCE_H

#include "neurala/image/dto/ImageMetadata.h"
#include "neurala/image/views/dto/ImageView.h"
#include "neurala/utils/Options.h"
#include "neurala/video/dto/CameraInfo.h"

namespace neurala
{
/**
 * @brief Base class for an abstact video data source. Plugin implementers should inherit from
 * this type.
 *
 * The sequence of calls can be expected to occur as follows:
 * 1. metadata()
 * 2. nextFrame()
 * 3. in case an error is returned:
 *     a) if it represents an overflow, the pipeline is stopped
 *     b) otherwise, the frame is skipped (go back to step 2.)
 * 4. frame() - either of the two
 * 5. go back to step 2.
 */
class VideoSource
{
public:
	/**
	 * For VideoSource implementations, the call to void* create(PluginArguments&,
	 * PluginErrorCallback&) will provide two plugin arguments:
	 *  1. dto::CameraInfo (information about the camera and how to connect to it, as provided through
	 * the implementation of CameraDiscoverer)
	 *  2. Options (additional camera settings - not mandatory)
	 */

	virtual ~VideoSource() = default;

	// Image dimension information
	[[nodiscard]] virtual dto::ImageMetadata metadata() const noexcept = 0;

	/**
	 * @brief Query availability of new frames​
	 *
	 * The request for the frame itself that follows will involve offering a view of the corresponding
	 * data. The process of making the frame available within the plugin should be resolved before
	 * returning for timely identification of errors.
	 *
	 * When this function is called, it is safe to assume potential resources for previous requests can
	 * be released.
	 */
	[[nodiscard]] virtual std::error_code nextFrame() noexcept = 0;

	/**
	 * @brief Get a frame from host memory
	 *
	 * Data needs to be valid until the frame is done processing. There is however no need for queues
	 * or other data structures. Work on a frame is finished by the time the next one is requested.
	 *
	 * Repetition in metadata requests between the dedicated function and as part of the ImageView's is
	 * intentional. Provided there are no frame to frame changes, the returned data should be the same.
	 */
	[[nodiscard]] virtual dto::ImageView frame() noexcept = 0;

	/**
	 * @brief Copy a frame into the buffer provided as argument
	 *
	 * Similar to the above declaration, yet in this case memory is managed automatically. The provided
	 * address should be used in constructing the returned ImageView.
	 *
	 * @param data address of the provided buffer
	 * @param capacity size limit of the provided buffer, expressed in bytes
	 */
	[[nodiscard]] virtual dto::ImageView frame(std::byte* data, std::size_t capacity) noexcept = 0;

	// Executes an arbitrary action on the video source
	[[nodiscard]] virtual std::error_code execute(const std::string& action) noexcept = 0;
};

} // namespace neurala

#endif // NEURALA_VIDEO_VIDEO_SOURCE_H
