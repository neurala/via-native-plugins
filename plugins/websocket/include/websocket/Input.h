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

#ifndef NEURALA_PLUG_WS_INPUT_H
#define NEURALA_PLUG_WS_INPUT_H

#include <cstddef>
#include <string>
#include <system_error>

#include <neurala/plugin/PluginArguments.h>
#include <neurala/plugin/PluginBindings.h>
#include <neurala/plugin/PluginErrorCallback.h>
#include <neurala/video/VideoSource.h>

#include "websocket/Client.h"

namespace neurala::plug::ws
{
/**
 * @brief Implementation of the VideoSource interface that retrieves frame data.
 */
class PLUGIN_API Input final : public VideoSource
{
public:
	static void* create(PluginArguments& args, PluginErrorCallback& ec)
	{
		try
		{
			return new Input;
		}
		catch (const std::system_error& se)
		{
			ec(se.code(), se.what());
		}
		catch (const std::exception& e)
		{
			ec(e.what());
		}
		catch (...)
		{
			ec("Could not create input interface");
		}

		return nullptr;
	}

	static void destroy(void* p) { delete reinterpret_cast<Input*>(p); }

	// Image dimension information
	[[nodiscard]] dto::ImageMetadata metadata() const noexcept final { return m_client.metadata(); }

	// Query new frames​
	[[nodiscard]] std::error_code nextFrame() noexcept final { return m_client.nextFrame(); }

	// Get a frame from host memory, data needs to be valid until the end of processing​
	[[nodiscard]] dto::ImageView frame() const noexcept final { return m_client.frame(); }

	// Copy a frame into the buffer provided as argument
	[[nodiscard]] dto::ImageView frame(std::byte* data, std::size_t size) const noexcept final;

	// Executes an arbitrary action on the video source
	[[nodiscard]] std::error_code execute(const std::string& action) noexcept final
	{
		return m_client.execute(action);
	}

private:
	mutable Client m_client;
};

} // namespace neurala::plug::ws

#endif // NEURALA_PLUG_WS_INPUT_H
