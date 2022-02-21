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

#ifndef NEURALA_VIDEO_VIDEO_SOURCE_STATUS_H
#define NEURALA_VIDEO_VIDEO_SOURCE_STATUS_H

#include <cstdint>
#include <ostream>
#include <system_error>

#include "neurala/exports.h"

namespace neurala
{
/**
 * @brief Type that describes the status of a VideoSource
 */
struct VideoSourceStatus final
{
	/// Operation was successful.
	static constexpr VideoSourceStatus success() { return {0}; }
	/// Unknown state.
	static constexpr VideoSourceStatus unknown() { return {1}; }
	/// Fetching frames timed out. This is considered an error.
	static constexpr VideoSourceStatus timeout() { return {2}; }
	/*
	 * Internal buffer overflow (e.g. frames not fetched in time).
	 * Currently buffered frames may still be fetched, however,
	 * so this may or may not be an error condition depending on
	 * whether skipped frames are allowed.
	 */
	static constexpr VideoSourceStatus overflow() { return {3}; }
	/*
	 * All other errors (requested frame number is outside valid range, no frame data
	 * available, VideoInput connection was closed, etc.).
	 */
	static constexpr VideoSourceStatus error() { return {4}; }
	/// Input format is not understood by the converters.
	static constexpr VideoSourceStatus pixelFormatNotSupported() { return {5}; }
	/// Information provided is not correct.
	static constexpr VideoSourceStatus invalidParameter() { return {6}; }
	/// Functionality is not implemented.
	static constexpr VideoSourceStatus notImplemented() { return {7}; }
	/// Functionality not supported.
	static constexpr VideoSourceStatus unsupportedAction() { return {8}; }

	constexpr operator int() const noexcept { return m_value; }

	int m_value;
};

NEURALA_PUBLIC const std::error_category& videoSourceStatusCategory() noexcept;
NEURALA_PUBLIC std::error_code make_error_code(VideoSourceStatus res) noexcept;
NEURALA_PUBLIC std::error_condition make_error_condition(VideoSourceStatus res) noexcept;

} // namespace neurala

namespace std
{
template<>
struct is_error_condition_enum<neurala::VideoSourceStatus> : true_type
{ };
} // namespace std

#endif // NEURALA_VIDEO_VIDEO_SOURCE_STATUS_H
