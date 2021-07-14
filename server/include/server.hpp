#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/config.hpp>

namespace neurala
{
namespace net = boost::asio;
namespace beast = boost::beast;
using tcp = net::ip::tcp;

/**
 * @brief Websocket server that offers mock image data.
 */
class Server final
{
public:
	Server(const std::string_view address, const std::uint16_t port);

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

	net::io_context m_ioContext;
	tcp::acceptor m_acceptor;

	struct Metadata final
	{
		std::size_t width;
		std::size_t height;
		std::string colorSpace;
		std::string layout;
		std::string dataType;
	} m_metadata;
};

} // namespace neurala
