/*
 * Copyright Neurala Inc. 2013-2023
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

#ifndef NEURALA_GSTREAMER_PLUGIN_H
#define NEURALA_GSTREAMER_PLUGIN_H

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>

#include "neurala/image/views/dto/ImageView.h"
#include "neurala/plugin/PluginArguments.h"
#include "neurala/plugin/PluginBindings.h"
#include "neurala/plugin/PluginErrorCallback.h"

#include "neurala/video/VideoSource.h"

namespace neurala
{
class GStreamerVideoSource : public VideoSource
{
private:
	struct Implementation;

	std::unique_ptr<Implementation> m_implementation;
	std::mutex m_mutex;
	std::condition_variable m_bufferReadyCondition;
	std::condition_variable m_frameReadyCondition;

	dto::ImageView m_frame;
	B4BError m_lastError;

	std::size_t m_width;
	std::size_t m_height;

	bool m_bufferReady;
	bool m_frameReady;
	bool m_endOfStream;

	bool bufferReady() const noexcept { return m_bufferReady; }
	bool frameReady() const noexcept { return m_frameReady || m_endOfStream; }

	static int preroll(void* sink, GStreamerVideoSource* self);
	static int grabFrame(void* sink, GStreamerVideoSource* self);

public:
	static void* create(PluginArguments&, PluginErrorCallback&);
	static void destroy(void* p);

	explicit GStreamerVideoSource(const char* name);

	~GStreamerVideoSource() noexcept;

	// Image dimension information
	[[nodiscard]] dto::ImageMetadata metadata() const noexcept;

	// Query new frames​
	[[nodiscard]] std::error_code nextFrame() noexcept;

	// Get a frame from host memory, data needs to be valid until the end of processing​
	[[nodiscard]] dto::ImageView frame() const noexcept;

	// Copy a frame into the buffer provided as argument
	[[nodiscard]] dto::ImageView frame(std::byte*, std::size_t) const noexcept;

	// Executes an arbitrary action on the video source
	[[nodiscard]] std::error_code execute(const std::string&) noexcept;
};
} // namespace

#endif // NEURALA_GSTREAMER_PLUGIN_H
