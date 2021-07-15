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

#include "client.h"

#include <cstdlib>
#include <iostream>

#include <neurala/meta/enum.h>

namespace neurala
{
/// Interpret an std::size_t value in a byte string starting at the given address.
static std::size_t
atoz(const char* const str)
{
	return static_cast<std::size_t>(std::atoll(str));
}

ImageMetadata
Client::metadata()
{
	const net::const_buffer buffer{response("metadata")};
	const char* begin{reinterpret_cast<const char*>(buffer.data())};
	const char* const end{begin + buffer.size()};
	const std::size_t width{atoz(begin)};
	begin = parseMetadata(begin, end);
	const std::size_t height{atoz(begin)};
	begin = parseMetadata(begin, end);
	const EColorSpace colorSpace{
	 stringToEnum<EColorSpace>(std::string{begin, metadataLength(begin, end)})};
	begin = parseMetadata(begin, end);
	const EImageDataLayout imageDataLayout{
	 stringToEnum<EImageDataLayout>(std::string{begin, metadataLength(begin, end)})};
	begin = parseMetadata(begin, end);
	const EDatatype datatype{stringToEnum<EDatatype>(std::string{begin, metadataLength(begin, end)})};
	return {width, height, colorSpace, imageDataLayout, datatype};
}

void
Client::frame(std::byte* const location)
{
	net::const_buffer buffer = response("frame");
	std::copy_n(reinterpret_cast<const std::byte*>(buffer.data()), buffer.size(), location);
}

Client::Client() : m_context{}, m_socket{m_context}, m_stream{m_socket}
{
	boost::system::error_code ec;
	tcp::endpoint endpoint{net::ip::make_address(kIpAddress, ec), kPort};
	m_socket.connect(endpoint, ec);
	if (ec)
	{
		std::cerr << "Failed to connect: " << ec.message() << ".\n";
		std::exit(1);
	}
	std::cout << "Client connected.\n";
	m_stream.handshake(kIpAddress.data(), "/");
}

net::const_buffer
Client::response(const std::string_view request)
{
	m_buffer.clear();
	m_stream.write(net::buffer(request));
	m_stream.read(m_buffer);
	const net::const_buffer data = m_buffer.cdata();
	if (data.size() < 1024)
	{
		std::cout << beast::make_printable(data) << '\n';
	}
	return data;
}

} // namespace neurala
