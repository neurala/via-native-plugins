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

#ifndef NEURALA_VIDEO_VIDEO_SOURCE_STATUS_H
#define NEURALA_VIDEO_VIDEO_SOURCE_STATUS_H

#include <cstdint>
#include <ostream>
#include <system_error>

#include "neurala/exports.h"
#include "neurala/meta/enum.h"

namespace neurala
{
/**
 * @brief Enum that describes the status of a VideoSource
 */
enum class VideoSourceStatus
{
	/// Operation was successful.
	success,
	/// Fetching frames timed out.  This is considered an error.
	timeout,
	/*
	 * Internal buffer overflow (e.g. frames not fetched in time).
	 * Currently buffered frames may still be fetched, however,
	 * so this may or may not be an error condition depending on
	 * whether skipped frames are allowed.
	 */
	overflow,
	/*
	 * All other errors.  (E.x. Requested frame number is outside valid range, no frame data
	 * available, VideoInput connection was closed...)
	 */
	error,
	/// Input format is not understood by the converters
	pixelFormatNotSupported,
	/// Information provided is not correct
	invalidParameter,
	/// Functionality is not implemented
	notImplemented,
	/// Functionality not supported
	unsupportedAction,
	/// Unknown state.
	unknown
};

NEURALA_PUBLIC const std::error_category& videoSourceStatusCategory() noexcept;
NEURALA_PUBLIC std::error_code make_error_code(VideoSourceStatus res) noexcept;
NEURALA_PUBLIC std::error_condition make_error_condition(VideoSourceStatus res) noexcept;

template<>
class MetaEnum<VideoSourceStatus> : public MetaEnumRegister<VideoSourceStatus>
{
public:
	static constexpr const auto values = enumRegisterValues(
	 NEURALA_META_ENUM_ENTRY(VideoSourceStatus, success),
	 NEURALA_META_ENUM_ENTRY(VideoSourceStatus, timeout),
	 NEURALA_META_ENUM_ENTRY(VideoSourceStatus, overflow),
	 NEURALA_META_ENUM_ENTRY(VideoSourceStatus, error),
	 NEURALA_META_ENUM_ENTRY(VideoSourceStatus, pixelFormatNotSupported),
	 NEURALA_META_ENUM_ENTRY(VideoSourceStatus, invalidParameter),
	 NEURALA_META_ENUM_ENTRY(VideoSourceStatus, notImplemented),
	 NEURALA_META_ENUM_ENTRY(VideoSourceStatus, unsupportedAction),
	 NEURALA_META_ENUM_ENTRY(VideoSourceStatus, unknown));

	static constexpr const auto fallbackValue = values[0];
};

} // namespace neurala

namespace std
{
template<> struct is_error_condition_enum<neurala::VideoSourceStatus> : true_type {};
}

#endif // NEURALA_VIDEO_VIDEO_SOURCE_STATUS_H
