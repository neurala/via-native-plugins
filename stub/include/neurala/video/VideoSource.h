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

#ifndef NEURALA_VIDEO_VIDEO_SOURCE_H
#define NEURALA_VIDEO_VIDEO_SOURCE_H

#include "neurala/image/ImageMetadata.h"
#include "neurala/image/views/ImageView.h"
#include "neurala/utils/Option.h"
#include "neurala/video/CameraInfo.h"

namespace neurala
{
/**
 * @brief Base class for an abstact video data source.
 */
class VideoSource
{
public:
	explicit VideoSource(const CameraInfo& cameraInfo, const Option& cameraOptions = {}) { }

	VideoSource() = default;

	VideoSource(const VideoSource&) = default;
	VideoSource(VideoSource&&) = default;

	VideoSource& operator=(const VideoSource&) = default;
	VideoSource& operator=(VideoSource&&) = default;

	virtual ~VideoSource() = default;

	// Image dimension information
	[[nodiscard]] virtual ImageMetadata metadata() const noexcept = 0;

	// Query new frames​
	[[nodiscard]] virtual std::error_code nextFrame() noexcept = 0;

	// Get a frame from host memory, data needs to be valid until the end of processing​
	[[nodiscard]] virtual ImageView frame() noexcept = 0;

	// Copy a frame into the buffer provided as argument
	[[nodiscard]] virtual ImageView frame(std::byte* data, std::size_t size) noexcept = 0;

	// Executes an arbitrary action on the video source
	[[nodiscard]] virtual std::error_code execute(const std::string& action) noexcept = 0;
};

} // namespace neurala

#endif // NEURALA_VIDEO_VIDEO_SOURCE_H
