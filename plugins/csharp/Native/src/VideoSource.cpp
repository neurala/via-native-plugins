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

#include <vector>

#include "neurala/error/B4BError.h"

#include "Bridge.h"
#include "VideoSource.h"

namespace neurala
{
std::vector<dto::CameraInfo>
CSharpCameraDiscoverer::operator()() const noexcept
{
	return {{"DotnetNativePluginCameraId1",
	         "dotnetVideoSource",
	         "External Dotnet Camera #1",
	         "DotnetNativePluginCameraConnection1"}};
}

void*
CSharpCameraDiscoverer::create(PluginArguments&, PluginErrorCallback& error)
{
	return new CSharpCameraDiscoverer();
}

void
CSharpCameraDiscoverer::destroy(void* p)
{
	delete static_cast<CSharpCameraDiscoverer*>(p);
}

dto::ImageMetadata
CSharpVideoSource::metadata() const noexcept
{
	int width;
	int height;

	neurala::dotnet::video_source::getMetadata(width, height);

	fprintf(stderr, "[width = %d, height = %d]\n", width, height);

	return dto::ImageMetadata("uint8", width, height, "BGR", "interleaved", "unknown");
}

std::error_code
CSharpVideoSource::nextFrame() noexcept
{
	std::int32_t status;

	neurala::dotnet::video_source::moveNextFrame(status);

	{
		const auto frameMetadata = metadata();

		imageBytes.assign(frameMetadata.width() * frameMetadata.height() * 4, std::byte(0));

		const auto buffer = imageBytes.data();

		neurala::dotnet::video_source::getFrame(buffer);

		currentFrame = dto::ImageView(frameMetadata, buffer);
	}

	const auto error = status ? B4BError::unknown() : B4BError::ok();

	return make_error_code(error);
}

dto::ImageView
CSharpVideoSource::frame() const noexcept
{
	const auto frameMetadata = metadata();
	const auto buffer = currentFrame.data();

	return dto::ImageView(metadata(), buffer);
}

dto::ImageView
CSharpVideoSource::frame(std::byte* data, std::size_t capacity) const noexcept
{
	imageBytes.assign(data, data + capacity);

	const auto buffer = imageBytes.data();

	return currentFrame = dto::ImageView(metadata(), buffer);
}

std::error_code
CSharpVideoSource::execute(const std::string& action) noexcept
{
	const auto string = action.c_str();

	neurala::dotnet::video_source::execute(string);

	return B4BError::ok();
}

void*
CSharpVideoSource::create(PluginArguments& arguments, PluginErrorCallback& error)
{
	return new CSharpVideoSource();
}

void
CSharpVideoSource::destroy(void* p)
{
	delete static_cast<CSharpVideoSource*>(p);
}
} // namespace neurala

