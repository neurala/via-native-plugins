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

#include "Client.h"

#include <cstdlib>
#include <iostream>

namespace neurala::plug::ws
{
Client::Client(const std::string_view ipAddress, const std::uint16_t port)
 : m_ioContext{}, m_socket{m_ioContext}, m_stream{m_socket}
{
	boost::system::error_code ec;
	tcp::endpoint endpoint{net::ip::make_address(ipAddress, ec), port};
	m_socket.connect(endpoint, ec);
	if (ec.failed())
	{
		std::cerr << "Failed to connect: " << ec.message() << ".\n";
		throw std::system_error(ec);
	}
	std::cout << "Client connected.\n";
	m_stream.handshake(ipAddress.data(), "/");
}

dto::ImageMetadata
Client::metadata()
{
	const net::const_buffer buffer{response("metadata")};
	using namespace boost::json;
	const value jsonValue{
	 parse(string_view{reinterpret_cast<const char*>(buffer.data()), buffer.size()})};
	const object& jsonObject{jsonValue.as_object()};
	const string& colorSpace{jsonObject.at("colorSpace").as_string()};
	const string& layout{jsonObject.at("layout").as_string()};
	const string& dataType{jsonObject.at("dataType").as_string()};
	return {static_cast<std::size_t>(jsonObject.at("width").as_int64()),
	        static_cast<std::size_t>(jsonObject.at("height").as_int64()),
	        std::string{colorSpace.data(), colorSpace.size()},
	        std::string{layout.data(), layout.size()},
	        std::string{dataType.data(), dataType.size()}};
}

std::vector<std::byte>
Client::frame()
{
	net::const_buffer buffer = response("frame");
	std::vector<std::byte> ret(buffer.size());
	std::copy_n(reinterpret_cast<const std::byte*>(buffer.data()), buffer.size(), begin(ret));
	return ret;
}

net::const_buffer
Client::response(const std::string_view requestType, const boost::json::object& body /*= {}*/)
{
	m_buffer.clear();
	boost::json::object request;
	request["request"] = requestType.data();
	if (!body.empty())
	{
		request["body"] = body;
	}
	m_stream.write(net::buffer(serialize(request)));
	m_stream.read(m_buffer);
	const net::const_buffer data = m_buffer.cdata();
	if (data.size() < 1024)
	{
		std::cout << beast::make_printable(data) << '\n';
	}
	return data;
}

} // namespace neurala::plug::ws
