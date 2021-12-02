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

#ifndef NEURALA_PLUG_WS_CLIENT_H
#define NEURALA_PLUG_WS_CLIENT_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/config.hpp>
#include <boost/json.hpp>
#include <neurala/image/ImageMetadata.h>
#include <neurala/plugin/PluginBindings.h>

namespace neurala::plug::ws
{
/**
 * @brief WebSocket client that receives input frames from a specified server.
 */
class PLUGIN_API Client final
{
public:
	Client();

	Client(const Client&) = delete;
	Client(Client&&) = default;
	Client& operator=(const Client&) = delete;
	Client& operator=(Client&&) = delete; // the stream is not move-assignable

	/// Closes the WebSocket stream used to communicate with the server.
	~Client();

	/**
	 * @brief Retrieve metadata regarding frames fed by the server.
	 *
	 * The client parses a sequence of five attributes (width, height, color space, layout, data type)
	 * enclosed as a JSON object. The width and height attributes must be represented as numbers. The
	 * latter three are interpreted as the string encoding of an element from the corresponding enum.
	 */
	ImageMetadata metadata() noexcept;

	/**
	 * @brief Retrieve the next frame.
	 */
	std::error_code nextFrame() noexcept;

	/**
	 * @brief Returns the buffer holding the last frame retrieved.
	 */
	const std::vector<std::byte>& frame() const noexcept { return m_frame; }

	/**
	 * @brief Executes an arbitrary action on the video source.
	 * @param action label assigned to the commanded action
	 */
	std::error_code execute(const std::string_view action) noexcept;

	/**
	 * @brief Send the result of processing a frame back to the server.
	 * @param result body of the result sending request
	 */
	void sendResult(boost::json::object&& result) noexcept;

private:
	using ConstBuffer = boost::asio::const_buffer;

	/**
	 * @brief Retrieve the response for a given request.
	 *
	 * Requests used the JSON format. The request type is set as the "request" element. If a body
	 * object is specified, a "body" element is also included in the message.
	 */
	ConstBuffer response(const std::string_view requestType,
	                     boost::json::object&& body,
	                     std::error_code& ec) noexcept;

	ImageMetadata metadataFromFrame() noexcept;

	boost::asio::io_context m_ioContext;
	boost::asio::ip::tcp::socket m_socket;
	boost::beast::websocket::stream<boost::asio::ip::tcp::socket&> m_stream;
	boost::beast::flat_buffer m_buffer;
	std::string m_frameFormat;
	neurala::ImageMetadata m_frameMetadata;
	std::vector<std::byte> m_frame;
};

} // namespace neurala::plug::ws

#endif // NEURALA_PLUG_WS_CLIENT_H
