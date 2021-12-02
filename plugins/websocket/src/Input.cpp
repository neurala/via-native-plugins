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

#include "websocket/Input.h"
#include "neurala/video/VideoSourceStatus.h"

#include <iostream>

namespace neurala::plug::ws
{
ImageView
Input::frame(std::byte* data, std::size_t size) noexcept
{
	const std::vector<std::byte>& buffer{m_client.frame()};
	if (size < buffer.size())
	{
		std::cerr << "Insufficient capacity in B4B buffer.\n";
	}
	else
	{
		std::copy(cbegin(buffer), cend(buffer), data);
	}
	return {cachedMetadata(), data};
}

const ImageMetadata&
Input::cachedMetadata() const noexcept
{
	return m_client.frameMetadata();
}

} // namespace neurala::plug::ws
