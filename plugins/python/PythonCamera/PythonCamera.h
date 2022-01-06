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

#ifndef NEURALA_PYTHON_CAMERA_H
#define NEURALA_PYTHON_CAMERA_H

#include <cstddef>

// #include "neurala/plugin/PluginBindings.h"
// #include "neurala/plugin/detail/PluginManager.h"
// #include "neurala/video/CameraDiscoverer.h"
// #include "neurala/video/dto/CameraInfo.h"
// #include "neurala/utils/Options.h"

#include <neurala/plugin/PluginArguments.h>
#include <neurala/plugin/PluginBindings.h>
#include <neurala/plugin/PluginErrorCallback.h>
#include <neurala/plugin/PluginManager.h>
#include <neurala/plugin/PluginStatus.h>



#include <neurala/video/CameraDiscoverer.h>
#include <neurala/video/VideoSource.h>
#include <neurala/video/VideoSourceStatus.h>
#include <neurala/video/dto/CameraInfo.h>

#include "Python.h"


namespace neurala {
	namespace PythonCamera {
		class PLUGIN_API Discoverer : public neurala::CameraDiscoverer
		{
		public:
			[[nodiscard]] std::vector<neurala::dto::CameraInfo> operator()() const noexcept override;

			static void* create(neurala::PluginArguments&, neurala::PluginErrorCallback&);
			static void destroy(void* p);
		};

		class PLUGIN_API Source : public neurala::VideoSource
		{
		public:
			explicit Source(const neurala::dto::CameraInfo& cameraInfo, const neurala::Options& cameraOptions = {});

			[[nodiscard]] neurala::dto::ImageMetadata metadata() const noexcept override
			{
				return {300, 300, "RGB", "interleaved", "uint8"};
			}

			[[nodiscard]] std::error_code nextFrame() noexcept override
			{
				return neurala::make_error_code(neurala::VideoSourceStatus::success());
			}

			[[nodiscard]] neurala::dto::ImageView frame() const noexcept override
			{
				return {metadata(), m_frame.get()};
			}

			[[nodiscard]] neurala::dto::ImageView frame(std::byte* data, std::size_t size) const noexcept override;

			std::error_code execute(const std::string& action) noexcept override;

			static void* create(neurala::PluginArguments&, neurala::PluginErrorCallback&);
			static void destroy(void*);

		private:
			std::unique_ptr<std::uint8_t[]> m_frame;
		};

	} // namespace PythonCamera
} // namespace neurala

#endif // NEURALA_PYTHON_PLUGIN_H
