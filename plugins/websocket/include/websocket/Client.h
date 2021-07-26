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

#ifndef NEURALA_PLUG_WS_CLIENT_H
#define NEURALA_PLUG_WS_CLIENT_H

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <string>
#include <string_view>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/config.hpp>
#include <neurala/image/ImageMetadata.h>
#include <neurala/plugin/PluginBindings.h>

namespace neurala::plug::ws
{
namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = net::ip::tcp;

/**
 * @brief Websocket client that receives input frames from a local server.
 */
class PLUGIN_API Client final
{
public:
	Client(const std::string_view ipAddress, const std::uint16_t port);
	~Client() { m_stream.close(beast::websocket::close_code::normal); }

	/**
	 * @brief Retrieve metadata regarding frames fed by the server.
	 *
	 * The client parses a sequence of five attributes (width, height, color space, layout, data type)
	 * separated by semicolons. The width and height attributes must be represented as numbers. The
	 * latter three are interpreted as the string encoding of an element from the corresponding enum.
	 */
	ImageMetadata metadata();

	/// Retrieve the next frame and copy data to the specified location.
	bool frame(std::byte* const location, const std::size_t capacity);

	/// Send the result of processing a frame back to the server.
	void sendResult(const std::string& result) { response(result); }

private:
	/// Return a pointer to the start of the next metadata element.
	static const char* parseMetadata(const char* const begin, const char* const end)
	{
		return std::next(std::find(begin, end, ';'));
	}

	/// Return the length of the current metadata element.
	static std::size_t metadataLength(const char* const begin, const char* const end)
	{
		return std::distance(begin, std::find(begin, end, ';'));
	}

	/// Retrieve the response for a given request.
	net::const_buffer response(const std::string_view request);

	net::io_context m_ioContext;
	tcp::socket m_socket;
	beast::websocket::stream<tcp::socket&> m_stream;
	beast::flat_buffer m_buffer;
};

} // namespace neurala::plug::ws

#endif // NEURALA_PLUG_WS_CLIENT_H
