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
template<>
struct is_error_condition_enum<neurala::VideoSourceStatus> : true_type
{ };
} // namespace std

#endif // NEURALA_VIDEO_VIDEO_SOURCE_STATUS_H
