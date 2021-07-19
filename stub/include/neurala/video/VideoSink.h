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

#ifndef NEURALA_VIDEO_VIDEO_SINK_H
#define NEURALA_VIDEO_VIDEO_SINK_H

#include "neurala/utils/Option.h"
#include "neurala/image/ImageMetadata.h"
#include "neurala/image/views/ImageView.h"
#include "neurala/video/CameraInfo.h"

namespace neurala
{
/**
 * @brief Base class for different video inputs.
 */
class VideoSink
{
public:
	explicit VideoSink(const CameraInfo& cameraInfo, const Option& cameraOptions = {}) { }

	VideoSink() = default;

	VideoSink(const VideoSink&) = default;
	VideoSink(VideoSink&&) = default;

	VideoSink& operator=(const VideoSink&) = default;
	VideoSink& operator=(VideoSink&&) = default;

	virtual ~VideoSink() = default;
	
	// Image dimension information
	[[nodiscard]] virtual ImageMetadata metadata() const = 0;

	// Push new frame
	virtual void load(const ImageView& image) = 0;
};

} // namespace neurala

#endif // NEURALA_VIDEO_VIDEO_SINK_H