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

#include "websocket/Client.h"
#include "websocket/Environment.h"

#include <cstdlib>
#include <iostream>
#include <utility>

#include <neurala/meta/enum.h>
#include <neurala/video/VideoSourceStatus.h>

namespace neurala::plug::ws
{
Client::Client() : m_ioContext{}, m_socket{m_ioContext}, m_stream{m_socket}
{
	try
	{
		boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::make_address(ipAddress), port};
		m_socket.connect(endpoint);
		std::clog << "WebSocket client connected.\n";
		m_stream.handshake(ipAddress.data(), "/");
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

ImageMetadata
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
		const value jsonValue{
		 parse(string_view{reinterpret_cast<const char*>(buffer.data()), buffer.size()})};
		const object& jsonObject{jsonValue.as_object()};
		const std::size_t width{static_cast<std::size_t>(jsonObject.at("width").as_int64())};
		const std::size_t height{static_cast<std::size_t>(jsonObject.at("height").as_int64())};
		const string& colorSpaceStr{jsonObject.at("colorSpace").as_string()};
		const EColorSpace colorSpace{
		 stringToEnum<EColorSpace>(std::string_view{colorSpaceStr.data(), colorSpaceStr.size()})};
		const string& layoutStr{jsonObject.at("layout").as_string()};
		const EImageDataLayout layout{
		 stringToEnum<EImageDataLayout>(std::string_view{layoutStr.data(), layoutStr.size()})};
		const string& dataTypeStr{jsonObject.at("dataType").as_string()};
		const EDatatype dataType{
		 stringToEnum<EDatatype>(std::string_view{dataTypeStr.data(), dataTypeStr.size()})};
		return {width, height, colorSpace, layout, dataType};
	}
	catch (...)
	{
		std::cerr << "Error while parsing 'metadata' response\n";
	}
	return {};
}

std::error_code
Client::frame(std::byte* const location, const std::size_t capacity) noexcept
{
	std::error_code ec;
	const ConstBuffer buffer{response("frame", {}, ec)};
	if (ec)
	{
		return ec;
	}
	const bool sufficientCapacity{buffer.size() <= capacity};
	if (sufficientCapacity)
	{
		std::copy_n(reinterpret_cast<const std::byte*>(buffer.data()), buffer.size(), location);
		return make_error_code(VideoSourceStatus::success);
	}
	return make_error_code(VideoSourceStatus::overflow);
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
		m_stream.write(boost::asio::buffer(serialize(request)));
		m_stream.read(m_buffer);
	}
	catch (const boost::beast::system_error& bse)
	{
		ec = std::make_error_code(static_cast<std::errc>(bse.code().value()));
		std::cerr << "Error while processing request: " << bse.what() << '\n';
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
