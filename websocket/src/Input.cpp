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
std::error_code
Input::nextFrame() noexcept
{
	std::vector<std::byte> frameBuffer(cachedMetadata().sizeBytes());
	const std::error_code ec{m_client.frame(frameBuffer.data(), frameBuffer.size())};
	m_frame = std::move(frameBuffer);
	return ec;
}

ImageView
Input::frame(std::byte* data, std::size_t size) noexcept
{
	const ImageMetadata& md{cachedMetadata()};
	if (size < md.sizeBytes())
	{
		std::cerr << "Insufficient capacity in B4B buffer.\n";
	}
	else
	{
		std::copy(cbegin(m_frame), cend(m_frame), data);
	}
	return {md, data};
}

std::error_code
Input::execute(const std::string& action) noexcept
{
	return m_client.execute(action);
}

const ImageMetadata&
Input::cachedMetadata() const noexcept
{
	if (!m_metadata)
	{
		m_metadata = m_client.metadata();
	}
	return *m_metadata;
}

} // namespace neurala::plug::ws
