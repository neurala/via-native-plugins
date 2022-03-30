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

#ifndef NEURALA_CSHARP_VIDEO_SOURCE_H
#define NEURALA_CSHARP_VIDEO_SOURCE_H

#include "neurala/video/VideoSource.h"

namespace neurala
{
class
#ifdef _WIN32
__declspec(dllexport)
#endif
CSharpVideoSource : public VideoSource
{
public:
	using Method = int (*)(void*, std::int32_t);

private:
	Method metadataGetter;
	Method nextFramer;
	Method frameGetter;
	Method executor;

public:
	explicit CSharpVideoSource(Method metadataGetter,
	                           Method nextFramer,
	                           Method frameGetter,
	                           Method executor) noexcept
	 : metadataGetter(metadataGetter),
	   nextFramer(nextFramer),
	   frameGetter(frameGetter),
	   executor(executor)
	{ }

	[[nodiscard]] virtual dto::ImageMetadata metadata() const noexcept override;

	[[nodiscard]] virtual std::error_code nextFrame() noexcept override;

	[[nodiscard]] virtual dto::ImageView frame() const noexcept override;

	[[nodiscard]] virtual dto::ImageView frame(std::byte* data, std::size_t capacity) const noexcept override;

	[[nodiscard]] virtual std::error_code execute(const std::string& action) noexcept override;
};
} // namespace neurala

#endif // NEURALA_CSHARP_VIDEO_SOURCE_H
