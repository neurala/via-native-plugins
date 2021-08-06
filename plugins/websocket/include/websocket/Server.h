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

#include <cstdint>
#include <functional>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <boost/thread.hpp>
#include <neurala/plugin/PluginBindings.h>

namespace neurala::plug::ws
{
namespace net = boost::asio;
namespace beast = boost::beast;
using tcp = net::ip::tcp;

/**
 * @brief Base implementation for a WebSocket server.
 *
 * Specific behavior depending on client requests must be added for an instance to be useful.
 */
class PLUGIN_API Server
{
public:
	using WebSocketStream = beast::websocket::stream<tcp::socket>;
	using RequestHandler = std::function<void(WebSocketStream&, const boost::json::object&)>;

	/**
	 * @param ipAddress connection IP address
	 * @param port connection port
	 * @param requestHandlers list of mappings between expected headers and the corresponding way of
	 * responding to the request messages
	 */
	Server(const std::string_view ipAddress,
	       const std::uint16_t port,
	       std::vector<std::pair<std::string_view, RequestHandler>>&& requestHandlers);

	Server(const Server&) = delete;
	Server(Server&&) = default;
	Server& operator=(const Server&) = delete;
	Server& operator=(Server&&) = default;

	/// Stop accepting connections and stop all existing ones before destroying the object.
	virtual ~Server();

private:
	/// Deploy the server.
	void run();

	/// Handle communication with a particular client.
	void session(tcp::socket&& socket);

	/**
	 * @brief Handle a particular request made by a client.
	 *
	 * Requests must use the JSON format. A "request" element representing the type is required. If a
	 * "body" element is also present, it gets passed to the corresponding handler function.
	 */
	void handleRequest(WebSocketStream& stream);

	std::unordered_map<std::string_view, RequestHandler> m_requestHandlers;
	net::io_context m_ioContext;
	tcp::acceptor m_acceptor;
	std::vector<boost::thread> m_sessions;
	volatile bool m_running;
	boost::thread m_thread;
};

} // namespace neurala::plug::ws

#endif // NEURALA_PLUG_WS_SERVER_H
