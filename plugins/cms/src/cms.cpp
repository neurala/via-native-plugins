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

#include "cms.h"
#include "cmsMultispectralLink.h"

#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <mutex>
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

std::unique_ptr<cmsMultiSpectralLink> multiSpectralLink;
std::mutex cmsLock;
std::condition_variable cmsCV;

neurala::dto::ImageMetadata cmsMetadata;
neurala::dto::ImageView cmsFrame;

bool waitingForFrame;

void
cmsEventHandler()
{
	{
		std::scoped_lock guard(cmsLock);

		if (waitingForFrame)
		{
			waitingForFrame = false;
			multiSpectralLink->getCmsImages(0)->setImageRaw(); //set the image raw
			multiSpectralLink->getCmsImages(0)->calcImageMultisprectralCube(); //computing the multispectral cube
			multiSpectralLink->getCmsImages(0)->calcImageColor(); //computing of the RGB and BGR images

			const auto display = multiSpectralLink->getCmsImages(0)->getImageBGRDisplay();

			cmsMetadata = neurala::dto::ImageMetadata("uint8", display->width, display->height, "BGR", "interleaved", "topLeft");
			cmsFrame = neurala::dto::ImageView(cmsMetadata, display->imageData);
		}
	}

	cmsCV.notify_all();
}
} // namespace

extern "C" PLUGIN_API NeuralaPluginExitFunction
initMe(NeuralaPluginManager* pluginManager, std::error_code* status)
{
	auto& pm = *dynamic_cast<neurala::PluginRegistrar*>(pluginManager);
	*status = pm.registerPlugin<neurala::plug::cms::CMSSource>("cmsVideoSource", neurala::Version(1, 0));
	if (*status != neurala::PluginStatus::success())
	{
		return nullptr;
	}

	*status = pm.registerPlugin<neurala::plug::cms::CMSDiscoverer>("cmsDiscoverer",
	                                                               neurala::Version(1, 0));
	if (*status != neurala::PluginStatus::success())
	{
		return nullptr;
	}

	return exitHere;
}

namespace neurala::plug::cms
{
std::vector<dto::CameraInfo>
CMSDiscoverer::operator()() const noexcept
{
	if (!multiSpectralLink)
	{
		multiSpectralLink = std::make_unique<cmsMultiSpectralLink>();
		multiSpectralLink->getCmsCamera()->setFrameRate(10.0); // Make this configurable.
		multiSpectralLink->getCmsCamera()->setEvent(cmsEventHandler);

		if (multiSpectralLink->getCmsInfos()->getSNCMS() == "")
		{
			puts("XML File Missing in ResourcesCMS");
			return std::vector<dto::CameraInfo>();
		}
	}

	const auto id = multiSpectralLink->getCmsInfos()->getTypeCamera().c_str();

	std::vector<dto::CameraInfo> cameraInformation;
	cameraInformation.emplace_back(id, "cmsVideoSource", "TOUCAN Multispectral Camera", "0");
	return cameraInformation;
}

void*
CMSDiscoverer::create(PluginArguments&, PluginErrorCallback& error)
{
	try
	{
		return new CMSDiscoverer();
	}
	catch (const std::exception& e)
	{
		error(e.what());
		return nullptr;
	}
}

void
CMSDiscoverer::destroy(void* p)
{
	delete static_cast<CMSDiscoverer*>(p);
}

CMSSource::CMSSource()
{
	
}

dto::ImageMetadata
CMSSource::metadata() const noexcept
{
	return cmsMetadata;
}

std::error_code
CMSSource::nextFrame() noexcept
{
	using namespace std::chrono_literals;

	std::unique_lock<std::mutex> lock(cmsLock);

	waitingForFrame = true;

	// Make this timeout configurable.
	const auto deadline = std::chrono::steady_clock::now() + 30s;

	while (true)
	{
		const auto status = cmsCV.wait_until(lock, deadline);

		if (status == std::cv_status::no_timeout && !waitingForFrame)
		{
			break;
		}
		if (status == std::cv_status::timeout)
		{
			break;
		}
	}

	const auto status = std::exchange(waitingForFrame, false) ? VideoSourceStatus::timeout() : VideoSourceStatus::success();

	return make_error_code(status);
}

dto::ImageView
CMSSource::frame() const noexcept
{
	return cmsFrame;
}

dto::ImageView
CMSSource::frame(std::byte* data, std::size_t size) const noexcept
{
	return cmsFrame = dto::ImageView(metadata(), data);
}

std::error_code
CMSSource::execute(const std::string& action) noexcept
{
	return std::error_code();
}

void*
CMSSource::create(PluginArguments& arguments, PluginErrorCallback& error)
{
	VideoSource* p = nullptr;

	try
	{
		p = new CMSSource();
	}
	catch (const std::exception& e)
	{
		error(e.what());
	}

	return p;
}

void
CMSSource::destroy(void* p)
{
	delete static_cast<VideoSource*>(p);
}

} // namespace neurala::plug::dummy
