/*
 * This file is part of Neurala SDK.
 * Copyright Neurala Inc. 2013-2021. All rights reserved.
 *
 * Except as expressly permitted in the accompanying License Agreement, if at all, (a) you shall
 * not license, sell, rent, lease, transfer, assign, distribute, display, host, outsource, disclose
 * or otherwise commercially exploit or make this source code available to any third party; (b) you
 * shall not modify, make derivative works of, disassemble, reverse compile or reverse engineer any
 * part of the SDK; (c) You shall not access the SDK in order to build a similar or competitive
 * product or service; (d) no part of the this source may be copied, reproduced, distributed,
 * republished, downloaded, displayed, posted or transmitted in any form or by any means, including
 * but not limited to electronic, mechanical, photocopying, recording or other means; and (e) any
 * future release, update, or other addition to functionality of the SDK shall be subject to the
 * terms of the accompanying License Agreement. You must reproduce, on all copies made by you or
 * for you, and must not remove, alter, or obscure in any way all proprietary rights notices
 * (including copyright notices) of Neurala Inc or its suppliers on or within the copies of the
 * SDK. Any sample code provided with the SDK and designated as such are for illustrative purposes
 * only and are not to be included in your applications.
 *
 * In cases when the accompanying License Agreement permits redistribution of this file, the above
 * notice shall be reproduced its entirety in every copy of a distributed version of this file.
 */

#include <iostream>
#include <numeric>

#include "neurala/utils/Version.h"
#include "neurala/video/CameraInfo.h"
#include "neurala/plugin/PluginManager.h"
#include "neurala/plugin/PluginBindings.h"
#include "neurala/plugin/PluginArguments.h"
#include "neurala/plugin/PluginErrorCallback.h"

#include "dummy.h"

namespace
{

int
exitHere()
{
	return 0;
}

} // namespace

extern "C" PLUGIN_API NeuralaPluginExitFunction
initMe(NeuralaPluginManager* pluginManager, NeuralaPluginStatus* status)
{
	auto& pm = *static_cast<neurala::PluginRegistrar*>(pluginManager);
	*status = pm.registerPlugin<neurala::plug::dummy::Source>("dummyVideoSource",
	                                                          neurala::Version(1, 0));
	if (*status != NeuralaPluginStatus::success)
	{
		return nullptr;
	}

	*status = pm.registerPlugin<neurala::plug::dummy::Discoverer>("dummyDiscoverer",
	                                                              neurala::Version(1, 0));
	if (*status != NeuralaPluginStatus::success)
	{
		return nullptr;
	}

	return exitHere;
}

namespace neurala::plug::dummy
{

std::vector<CameraInfo>
Discoverer::operator()() const
{
	std::cout << "Discovering available cameras...\n";
	return {CameraInfo("DummyNativePluginCamera", ECameraType::unknown, "External Dummy Camera", "")};
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

Source::Source(const CameraInfo& cameraInfo, const Option& options)
{
	std::cout << "Initiating VideoSource connection with " << cameraInfo << '\n';
	std::cout << "With options: " << options << '\n';

	const auto frameBufferSize = metadata().pixelComponentCount();
	m_frame = std::make_unique<std::uint8_t[]>(frameBufferSize);
	std::iota(&(m_frame[0]), &(m_frame[frameBufferSize]), 0);
}

ImageView
Source::frame(std::byte* data, std::size_t size)
{
	std::cout << "Copying frame to [" << data << "]\n";
	std::copy_n(reinterpret_cast<const std::byte*>(frame().data()), frame().sizeBytes(), data);
	return ImageView{metadata(), data};
}

std::error_code
Source::execute(const std::string& action)
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
		const auto& cameraInfo = arguments.get<0, const CameraInfo>();
		const auto& cameraOptions = arguments.get<1, const Option>();

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
