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

#ifndef NEURALA_DUMMY_PLUGIN_H
#define NEURALA_DUMMY_PLUGIN_H

#include "neurala/plugin/PluginBindings.h"

#include "neurala/video/CameraDiscoverer.h"
#include "neurala/video/VideoSource.h"
#include "neurala/video/VideoSourceStatus.h"

namespace neurala
{
class PluginArguments;
class PluginErrorCallback;

namespace plug::dummy
{
class PLUGIN_API Discoverer : public CameraDiscoverer
{
public:
	[[nodiscard]] std::vector<CameraInfo> operator()() const noexcept override;

	static void* create(PluginArguments&, PluginErrorCallback&);
	static void destroy(void* p);
};

/**
 * @brief Dummy plugin video input based on VideoInputOCV
 */
class PLUGIN_API Source : public VideoSource
{
public:
	explicit Source(const CameraInfo& cameraInfo, const Options& cameraOptions = {});

	[[nodiscard]] ImageMetadata metadata() const noexcept override
	{
		return ImageMetadata(200, 200, EColorSpace::RGB, EImageDataLayout::planar, EDatatype::uint8);
	}

	[[nodiscard]] std::error_code nextFrame() noexcept override
	{
		return make_error_code(VideoSourceStatus::success());
	}

	[[nodiscard]] ImageView frame() const noexcept override
	{
		return ImageView(metadata(), m_frame.get());
	}

	[[nodiscard]] ImageView frame(std::byte* data, std::size_t size) const noexcept override;

	std::error_code execute(const std::string& action) noexcept override;

	static void* create(PluginArguments&, PluginErrorCallback&);
	static void destroy(void*);

private:
	std::unique_ptr<std::uint8_t[]> m_frame;
};

} // namespace plug::dummy

} // namespace neurala

#endif // NEURALA_DUMMY_PLUGIN_H
