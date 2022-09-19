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

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <tuple>
#include <utility>

#include <neurala/video/VideoSourceStatus.h>

#include "websocket/Client.h"
#include "websocket/Environment.h"
#include <boost/algorithm/string/replace.hpp>

namespace neurala::plug::ws
{
Client::Client() : m_ioContext{}, m_socket{m_ioContext}, m_stream{m_socket}, m_frameCache{}
{
	try
	{
		boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::make_address(ipAddress), port};
		m_socket.connect(endpoint);
		// Set buffer limit to just above 4k images
		const auto previousMax = m_stream.read_message_max();
		m_stream.read_message_max(280000000);
		std::clog << "Changed max message: " << previousMax << " -> " << m_stream.read_message_max()
		          << '\n';
		m_stream.handshake(ipAddress.data(), "/");
		std::clog << "WebSocket client connected.\n";
		m_frameCache.metadata = metadata(); // ensure the cache is initialized
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error while initializing WebSocket connection: " << e.what() << '\n';
	}
	catch (...)
	{
		std::cerr << "Unknown error while initializing WebSocket connection.\n";
	}
}

Client::~Client()
{
	boost::system::error_code ec;
	m_stream.close(boost::beast::websocket::close_code::normal, ec);
	if (ec)
	{
		std::cerr << "Error while disconnecting from the server: " << ec.message() << '\n';
	}
}

dto::ImageMetadata
Client::metadata() noexcept
{
	std::error_code ec;
	const ConstBuffer buffer{response("metadata", {}, ec)};
	if (ec)
	{
		return {};
	}
	try
	{
		using namespace boost::json;
		parser json_parser;
		json_parser.write(string_view{reinterpret_cast<const char*>(buffer.data()), buffer.size()});
		value jsonValue = json_parser.release();

		const object& jsonObject{jsonValue.as_object()};

		if (jsonObject.contains("format"))
		{
			const auto& s{jsonObject.at("format").as_string()};
			m_frameCache.format = std::string{s.data(), s.size()};
			const std::size_t width{static_cast<std::size_t>(jsonObject.at("width").as_int64())};
			const std::size_t height{static_cast<std::size_t>(jsonObject.at("height").as_int64())};
			return {"uint8", width, height, "RGB", "interleaved", "topLeft"};
		}

		const string& dataType{jsonObject.at("dataType").as_string()};
		const std::size_t width{static_cast<std::size_t>(jsonObject.at("width").as_int64())};
		const std::size_t height{static_cast<std::size_t>(jsonObject.at("height").as_int64())};
		const string& colorSpace{jsonObject.at("colorSpace").as_string()};
		const string& layout{jsonObject.at("layout").as_string()};
		const string& orientation{jsonObject.at("orientation").as_string()};
		return {std::string{dataType.data(), dataType.size()},
		        width,
		        height,
		        std::string{colorSpace.data(), colorSpace.size()},
		        std::string{layout.data(), layout.size()},
		        std::string{orientation.data(), orientation.size()}};
	}
	catch (...)
	{
		std::cerr << "Error while parsing 'metadata' response\n";
	}
	return {};
}

std::error_code
Client::nextFrame() noexcept
{
	std::error_code ec;
	const ConstBuffer buffer{response("frame", {}, ec)};
	if (ec)
	{
		return ec;
	}
	if (m_frameCache.format.empty())
	{
		m_frameCache.data.resize(buffer.size());
		std::copy_n(reinterpret_cast<const std::byte*>(buffer.data()),
		            buffer.size(),
		            begin(m_frameCache.data));
		return make_error_code(VideoSourceStatus::success());
	}
	return make_error_code(VideoSourceStatus::pixelFormatNotSupported());
}

std::error_code
Client::execute(const std::string_view action) noexcept
{
	std::error_code ec;
	response("execute", {{"action", action.data()}}, ec);
	return ec;
}

void
Client::sendResult(boost::json::object&& result) noexcept
{
	std::error_code ec;
	response("result", std::move(result), ec);
}

Client::ConstBuffer
Client::response(const std::string_view requestType,
                 boost::json::object&& body,
                 std::error_code& ec) noexcept
{
	m_buffer.clear();
	try
	{
		boost::json::object request{{"request", requestType.data()}};
		if (!body.empty())
		{
			request.emplace("body", std::move(body));
		}
		auto str = serialize(request);
		m_stream.write(boost::asio::buffer(str));
		m_stream.read(m_buffer);
	}
	catch (const boost::beast::system_error& se)
	{
		ec = std::make_error_code(static_cast<std::errc>(se.code().value()));
		std::cerr << "Error while processing request: " << se.what() << '\n';
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error while processing request: " << e.what() << '\n';
	}
	catch (...)
	{
		std::cerr << "Unknown error while processing request.\n";
	}
	return m_buffer.cdata();
}

} // namespace neurala::plug::ws
