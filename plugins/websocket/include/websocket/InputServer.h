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

#ifndef NEURALA_PLUG_WS_INPUT_SERVER_H
#define NEURALA_PLUG_WS_INPUT_SERVER_H

#include <cstddef>
#include <cstdint>
#include <string_view>

#include <neurala/plugin/PluginBindings.h>

#include "Server.h"

namespace neurala::plug::ws
{
/**
 * @brief Implementation of the server base that handles metadata and frame requests.
 */
class PLUGIN_API InputServer final : public Server
{
public:
	InputServer(const std::string_view ipAddress, const std::uint16_t port);

private:
	/// Handle an image metadata request.
	void handleMetadata(WebSocketStream& stream);
	/// Handle a frame request.
	void handleFrame(WebSocketStream& stream);

	struct Metadata final
	{
		std::size_t width;
		std::size_t height;
		std::string_view colorSpace;
		std::string_view layout;
		std::string_view dataType;
	} m_metadata;
};

} // namespace neurala::plug::ws

#endif // NEURALA_PLUG_WS_INPUT_SERVER_H
