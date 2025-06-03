/*
 * Copyright Neurala Inc. 2013-2025
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
class Server
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
	bool m_running;
	boost::thread m_thread;
};

} // namespace neurala::plug::ws

#endif // NEURALA_PLUG_WS_SERVER_H
