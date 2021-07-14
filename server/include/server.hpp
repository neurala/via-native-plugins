#pragma once

#include <string_view>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/config.hpp>

namespace neurala
{
namespace net = boost::asio;
namespace beast = boost::beast;
using tcp = net::ip::tcp;

class Server final
{
public:
	Server(const std::string_view address, const std::uint16_t port);

private:
	void run();

	void session(tcp::socket&& socket);
	void handleRequest(beast::websocket::stream<tcp::socket>& stream);

	void handleMetadata(beast::websocket::stream<tcp::socket>& stream);
	void handleFrame(beast::websocket::stream<tcp::socket>& stream);
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
