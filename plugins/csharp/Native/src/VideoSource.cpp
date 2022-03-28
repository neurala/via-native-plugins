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

#include "neurala/error/B4BError.h"

#include "VideoSource.h"

namespace neurala
{
dto::ImageMetadata
CSharpVideoSource::metadata() const noexcept
{
	struct {
		std::int32_t width;
		std::int32_t height;
	} arguments;

	metadataGetter(&arguments, sizeof arguments);

	return dto::ImageMetadata(arguments.width, arguments.height, "", "", "");
}

std::error_code
CSharpVideoSource::nextFrame() noexcept
{
	std::int32_t status;

	nextFramer(&status, sizeof status);

	const auto error = status ? B4BError::unknown() : B4BError::ok();

	return make_error_code(error);
}

dto::ImageView
CSharpVideoSource::frame() const noexcept
{
	struct {
		void* data;
	} arguments;

	frameGetter(&arguments, sizeof arguments);

	return dto::ImageView(metadata(), arguments.data);
}

dto::ImageView
CSharpVideoSource::frame(std::byte* data, std::size_t capacity) const noexcept
{
	std::abort();
}

std::error_code
CSharpVideoSource::execute(const std::string& action) noexcept
{
	struct {
		const char* action;
	} arguments;

	arguments.action = action.c_str();

	executor(&arguments, sizeof arguments);
}
} // namespace neurala

