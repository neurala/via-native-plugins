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

#include "dummy.h"

#include <iostream>
#include <numeric>
#include <system_error>
#include <utility>

#include "neurala/plugin/PluginArguments.h"
#include "neurala/plugin/PluginBindings.h"
#include "neurala/plugin/PluginErrorCallback.h"
#include "neurala/plugin/PluginManager.h"
#include "neurala/plugin/PluginStatus.h"
#include "neurala/utils/Version.h"
#include "neurala/video/dto/CameraInfo.h"

namespace
{
int
exitHere()
{
	return 0;
}

constexpr auto kSourceTypeName = "dummyVideoSource";

} // namespace

extern "C" PLUGIN_API NeuralaPluginExitFunction
initMe(NeuralaPluginManager* pluginManager, std::error_code* status)
{
	auto& pm = *dynamic_cast<neurala::PluginRegistrar*>(pluginManager);
	*status = pm.registerPlugin<neurala::plug::dummy::Source>(kSourceTypeName, neurala::Version(1, 0));
	if (*status != neurala::PluginStatus::success)
	{
		return nullptr;
	}

	*status = pm.registerPlugin<neurala::plug::dummy::Discoverer>("dummyDiscoverer",
	                                                              neurala::Version(1, 0));
	if (*status != neurala::PluginStatus::success)
	{
		return nullptr;
	}

	return exitHere;
}

namespace neurala::plug::dummy
{
std::vector<dto::CameraInfo>
Discoverer::operator()() const noexcept
{
	std::cout << "Discovering available cameras...\n";
	return {{"DummyNativePluginCameraId1",
	         kSourceTypeName,
	         "External Dummy Camera #1",
	         "DummyNativePluginCameraConnection1"},
	        {"DummyNativePluginCameraId1",
	         kSourceTypeName,
	         "External Dummy Camera #2",
	         "DummyNativePluginCameraConnection2"}};
}

void*
Discoverer::create(PluginArguments&, PluginErrorCallback& error)
{
	Discoverer* p = nullptr;

	try
	{
		p = new Discoverer();
	}
	catch (const std::exception& e)
	{
		error(e.what());
	}

	return p;
}

void
Discoverer::destroy(void* p)
{
	delete static_cast<Discoverer*>(p);
}

Source::Source(const dto::CameraInfo& cameraInfo, const Options& options)
{
	std::cout << "Initiating VideoSource connection with " << cameraInfo << '\n';
	std::cout << "With options: " << options << '\n';

	const dto::ImageMetadata md{metadata()};
	const auto frameBufferSize{md.width() * md.height() * 3};
	m_frame = std::make_unique<std::uint8_t[]>(frameBufferSize);
	std::iota(&(m_frame[0]), &(m_frame[frameBufferSize]), 0);
}

dto::ImageView
Source::frame(std::byte* data, std::size_t size) noexcept
{
	std::cout << "Copying frame to [" << data << "]\n";
	dto::ImageMetadata md{metadata()};
	std::copy_n(reinterpret_cast<const std::byte*>(m_frame.get()), md.width() * md.height() * 3, data);
	return {std::move(metadata()), data};
}

std::error_code
Source::execute(const std::string& action) noexcept
{
	std::cout << "Executing action: " << action << '\n';
	return std::error_code{};
}

void*
Source::create(PluginArguments& arguments, PluginErrorCallback& error)
{
	VideoSource* p = nullptr;

	try
	{
		const auto& cameraInfo = arguments.get<0, const dto::CameraInfo>();
		const auto& cameraOptions = arguments.get<1, const Options>();

		p = new Source(cameraInfo, cameraOptions);
	}
	catch (const std::exception& e)
	{
		error(e.what());
	}

	return p;
}

void
Source::destroy(void* p)
{
	delete static_cast<VideoSource*>(p);
}

} // namespace neurala::plug::dummy
