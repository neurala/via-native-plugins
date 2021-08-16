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
#include "neurala/utils/Options.h"
#include "neurala/video/CameraInfo.h"

namespace neurala
{
/**
 * @brief Base class for an abstact video data source. Plugin implementers should inherit from
 * this type.
 *
 * The sequence of calls can be expected to occur as follows:
 * 1. metadata()
 * 2. nextFrame()
 * 3. in case an error is returned:
 *     a) if it represents an overflow, the pipeline is stopped
 *     b) otherwise, the frame is skipped (go back to step 2.)
 * 4. frame() - either of the two
 * 5. go back to step 2.
 */
class VideoSource
{
public:
	/**
	 * For VideoSource implementations, the call to void* create(PluginArguments&,
	 * PluginErrorCallback&) will provide two plugin arguments:
	 *  1. CameraInfo (information about the camera and how to connect to it, as provided through the
	 * implementation of CameraDiscoverer)
	 *  2. Options (additional camera settings - not mandatory)
	 */

	virtual ~VideoSource() = default;

	// Image dimension information
	[[nodiscard]] virtual ImageMetadata metadata() const noexcept = 0;

	/**
	 * @brief Query availability of new frames​
	 *
	 * The request for the frame itself that follows will involve offering a view of the corresponding
	 * data. The process of making the frame available within the plugin should be resolved before
	 * returning for timely identification of errors.
	 *
	 * When this function is called, it is safe to assume potential resources for previous requests can
	 * be released.
	 */
	[[nodiscard]] virtual std::error_code nextFrame() noexcept = 0;

	/**
	 * @brief Get a frame from host memory
	 *
	 * Data needs to be valid until the frame is done processing. There is however no need for queues
	 * or other data structures. Work on a frame is finished by the time the next one is requested.
	 *
	 * Repetition in metadata requests between the dedicated function and as part of the ImageView's is
	 * intentional. Provided there are no frame to frame changes, the returned data should be the same.
	 */
	[[nodiscard]] virtual ImageView frame() noexcept = 0;

	/**
	 * @brief Copy a frame into the buffer provided as argument
	 *
	 * Similar to the above declaration, yet in this case memory is managed automatically. The provided
	 * address should be used in constructing the returned ImageView.
	 *
	 * @param data address of the provided buffer
	 * @param capacity size limit of the provided buffer, expressed in bytes
	 */
	[[nodiscard]] virtual ImageView frame(std::byte* data, std::size_t capacity) noexcept = 0;

	// Executes an arbitrary action on the video source
	[[nodiscard]] virtual std::error_code execute(const std::string& action) noexcept = 0;
};

} // namespace neurala

#endif // NEURALA_VIDEO_VIDEO_SOURCE_H
