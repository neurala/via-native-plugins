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

#include "neurala/video/VideoSourceStatus.h"

#include "neurala/error/B4BError.h"

namespace neurala
{
namespace
{
class VideoSourceStatusCat final : public std::error_category
{
public:
	const char* name() const noexcept override { return "VideoSourceStatus"; }

	std::string message(int c) const override
	{
		return enumToString(static_cast<VideoSourceStatus>(c));
	}

	bool equivalent(int code, const std::error_condition& condition) const noexcept override
	{
		// Avoid infinite loops
		if (*this == condition.category())
		{
			return code == condition.value();
		}

		switch (static_cast<VideoSourceStatus>(code))
		{
			case VideoSourceStatus::success:
				return B4BError::ok == condition;
			case VideoSourceStatus::error:
				return B4BError::genericError == condition;
			case VideoSourceStatus::unknown:
				return B4BError::unknown == condition;
			default:
				return false;
		}
	}

	bool equivalent(const std::error_code& code, int condition) const noexcept override
	{
		// Avoid infinite loops
		if (code.category() == *this)
		{
			return code.value() == condition;
		}

		switch (static_cast<VideoSourceStatus>(condition))
		{
			case VideoSourceStatus::success:
				return code == B4BError::ok;
			case VideoSourceStatus::error:
				return code == B4BError::genericError;
			case VideoSourceStatus::unknown:
				return code == B4BError::unknown;
			default:
				return false;
		}
	}
};
} // namespace

const std::error_category&
videoSourceStatusCategory() noexcept
{
	static const VideoSourceStatusCat cat;
	return cat;
}


std::error_condition
make_error_condition(VideoSourceStatus res) noexcept
{
	return std::error_condition(static_cast<int>(res), videoSourceStatusCategory());
}


std::error_code
make_error_code(VideoSourceStatus res) noexcept
{
	return std::error_code(static_cast<int>(res), videoSourceStatusCategory());
}

} // namespace neurala
