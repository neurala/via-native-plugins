/*
 * Copyright Neurala Inc. 2013-2022
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

#ifndef NEURALA_CMS_PLUGIN_H
#define NEURALA_CMS_PLUGIN_H

#include "neurala/plugin/PluginBindings.h"

#include "neurala/video/CameraDiscoverer.h"
#include "neurala/video/VideoSource.h"
#include "neurala/video/VideoSourceStatus.h"

namespace neurala
{
class PluginArguments;
class PluginErrorCallback;

namespace plug::cms
{
class PLUGIN_API CMSDiscoverer : public CameraDiscoverer
{
public:
	[[nodiscard]] std::vector<dto::CameraInfo> operator()() const noexcept override;

	static void* create(PluginArguments&, PluginErrorCallback&);
	static void destroy(void* p);
};

class PLUGIN_API CMSSource : public VideoSource
{
public:
	explicit CMSSource();

	[[nodiscard]] dto::ImageMetadata metadata() const noexcept override;

	[[nodiscard]] std::error_code nextFrame() noexcept override;

	[[nodiscard]] dto::ImageView frame() const noexcept override;

	[[nodiscard]] dto::ImageView frame(std::byte* data, std::size_t size) const noexcept override;

	std::error_code execute(const std::string& action) noexcept override;

	static void* create(PluginArguments&, PluginErrorCallback&);
	static void destroy(void*);
};

} // namespace plug::cms

} // namespace neurala

#endif // NEURALA_CMS_PLUGIN_H
