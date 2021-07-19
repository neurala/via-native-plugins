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

#ifndef NEURALA_DUMMY_PLUGIN_H
#define NEURALA_DUMMY_PLUGIN_H

#include "neurala/plugin/PluginBindings.h"

#include "neurala/video/VideoSink.h"
#include "neurala/video/VideoSource.h"
#include "neurala/video/CameraDiscoverer.h"

namespace neurala
{

class PluginArguments;
class PluginErrorCallback;

class PLUGIN_API CameraDiscovererDummy : public CameraDiscoverer
{
public:
	[[nodiscard]] std::vector<CameraInfo> operator()() const override;

	static void* create(PluginArguments&, PluginErrorCallback&);
	static void destroy(void* p);
};

/**
 * @brief Dummy plugin video input that stores a single frame
 */
class PLUGIN_API DummyVideoSink : public VideoSink
{
public:
	explicit DummyVideoSink(const CameraInfo& cameraInfo, const Option& cameraOptions = {});

	[[nodiscard]] ImageMetadata metadata() const override { return ImageMetadata(200, 200, EColorSpace::RGB, EImageDataLayout::planar, EDatatype::uint8); }

	void load(const ImageView& image) override;

	static void* create(PluginArguments&, PluginErrorCallback&);
	static void destroy(void*);

private:
	std::unique_ptr<std::uint8_t[]> m_frame;
};

/**
 * @brief Dummy plugin video input based on VideoInputOCV
 */
class PLUGIN_API DummyVideoSource : public VideoSource
{
public:
	explicit DummyVideoSource(const CameraInfo& cameraInfo, const Option& cameraOptions = {});

	[[nodiscard]] ImageMetadata metadata() const override { return ImageMetadata(200, 200, EColorSpace::RGB, EImageDataLayout::planar, EDatatype::uint8); }

	[[nodiscard]] std::error_code nextFrame() override { return std::error_code{}; }

	[[nodiscard]] ImageView frame() override { return ImageView(metadata(), m_frame.get()); }

	[[nodiscard]] ImageView frame(std::byte* data, std::size_t size) override;

	std::error_code execute(const std::string& action) override;

	static void* create(PluginArguments&, PluginErrorCallback&);
	static void destroy(void*);

	static std::vector<CameraInfo> cameras();

private:
	std::unique_ptr<std::uint8_t[]> m_frame;
};

} // namespace neurala

#endif // NEURALA_DUMMY_PLUGIN_H
