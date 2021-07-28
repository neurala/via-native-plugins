/*
 * Copyright Neurala Inc. 2013-2021. All rights reserved.
 *
 * Except as expressly permitted in the accompanying License Agreement, if at all, (a) you shall
 * not license, sell, rent, lease, transfer, assign, distribute, display, host, outsource, disclose
 * or otherwise commercially exploit or make this source code available to any third party; (b) you
 * shall not modify, make derivative works of, disassemble, reverse compile or reverse engineer any
 * part of the SDK; (c) You shall not access the SDK in order to build a similar or competitive
 * product or service; (d) no part of the this source may be copied, reproduced, distributed,
 * republished, downloaded, displayed, posted or transmitted in any form or by any means, including
 * but not limited to electronic, mechanical, photocopying, recording or other means; and (e) any
 * future release, update, or other addition to functionality of the SDK shall be subject to the
 * terms of the accompanying License Agreement. You must reproduce, on all copies made by you or
 * for you, and must not remove, alter, or obscure in any way all proprietary rights notices
 * (including copyright notices) of Neurala Inc or its suppliers on or within the copies of the
 * SDK. Any sample code provided with the SDK and designated as such are for illustrative purposes
 * only and are not to be included in your applications.
 *
 * In cases when the accompanying License Agreement permits redistribution of this file, the above
 * notice shall be reproduced its entirety in every copy of a distributed version of this file.
 */

#include "Client.h"

#include <cstdlib>
#include <iostream>

#include <neurala/meta/enum.h>

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

ImageMetadata
Client::metadata()
{
	const net::const_buffer buffer{response("metadata")};
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

bool
Client::frame(std::byte* const location, const std::size_t capacity)
{
	net::const_buffer buffer = response("frame");
	const bool sufficientCapacity{buffer.size() <= capacity};
	if (sufficientCapacity)
	{
		std::copy_n(reinterpret_cast<const std::byte*>(buffer.data()), buffer.size(), location);
	}
	return sufficientCapacity;
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
