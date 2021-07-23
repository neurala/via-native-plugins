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

#ifndef NEURALA_PLUG_WS_SERVER_H
#define NEURALA_PLUG_WS_SERVER_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/config.hpp>
#include <boost/thread.hpp>
#include <neurala/plugin/PluginBindings.h>

namespace neurala::plug::ws
{
namespace net = boost::asio;
namespace beast = boost::beast;
using tcp = net::ip::tcp;

/**
 * @brief Websocket server that offers mock image data.
 */
class PLUGIN_API Server final
{
public:
	Server(const std::string_view address, const std::uint16_t port);
	~Server();

private:
	/// Deploy the server.
	void run();

	/// Handle communication with a particular client.
	void session(tcp::socket&& socket);

	/// Handle a particular request made by a client.
	void handleRequest(beast::websocket::stream<tcp::socket>& stream);

	/// Handle an image metadata request.
	void handleMetadata(beast::websocket::stream<tcp::socket>& stream);
	/// Handle a frame request.
	void handleFrame(beast::websocket::stream<tcp::socket>& stream);
	/// Handle a result JSON.
	void handleResult(beast::websocket::stream<tcp::socket>& stream);

	struct Metadata final
	{
		std::size_t width;
		std::size_t height;
		std::string colorSpace;
		std::string layout;
		std::string dataType;
	} m_metadata;

	net::io_context m_ioContext;
	tcp::acceptor m_acceptor;
	std::vector<boost::thread> m_sessions;
	volatile bool m_running;
	boost::thread m_thread;
};

} // namespace neurala::plug::ws

#endif // NEURALA_PLUG_WS_SERVER_H
