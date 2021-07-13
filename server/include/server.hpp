#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/config.hpp>

namespace neurala
{
namespace net = boost::asio;
namespace beast = boost::beast;
using tcp = net::ip::tcp;

void
do_session(tcp::socket socket)
{
	try
	{
		beast::websocket::stream<tcp::socket> ws{std::move(socket)};
		// ws.set_option(beast::websocket::stream_base::decorator(
		//     [](beast::websocket::response_type& res)
		//     {
		//         res.set(beast::http::field::server,
		//             std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-sync");
		//     }));
		ws.accept();
		static constexpr std::string_view metadata{"metadata"};
		static constexpr std::string_view frame{"frame"};
		static constexpr std::string_view result{"{ \"result\": \"success\" }"};
		while (true)
		{
			beast::flat_buffer buffer;
			ws.read(buffer);
			// ws.text(true);
			// ws.write(buffer.data());
			const char* bufferData{reinterpret_cast<const char*>(buffer.cdata().data())};
			if (std::equal(metadata.cbegin(), metadata.cend(), bufferData))
			{
				ws.write(net::buffer("800;600;RGB;planar;uint8;"));
			}
			else if (std::equal(frame.cbegin(), frame.cend(), bufferData))
			{
				ws.binary(true);
				ws.text(false);
				std::vector<std::uint8_t> frameData(2560 * 1440 * 4);
				std::iota(begin(frameData), end(frameData), 0);
				ws.write(net::buffer(frameData));
			}
			else if (std::equal(result.cbegin(), result.cend(), bufferData))
			{
				ws.write(net::buffer("DONE"));
			}
		}
	}
	catch (const beast::system_error& se)
	{
		if (se.code() != beast::websocket::error::closed)
			std::cerr << "Error: " << se.code().message() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

class Server final
{
public:
	Server(const std::string_view address, const std::uint16_t port)
	 : m_ioContext{1}, m_acceptor{m_ioContext, tcp::endpoint{net::ip::make_address(address), port}}
	{
		try
		{
			run();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}

private:
	void run()
	{
		while (true)
		{
			tcp::socket socket{m_ioContext};
			m_acceptor.accept(socket);
			std::thread(&do_session, std::move(socket)).detach();
		}
	}

	net::io_context m_ioContext;
	tcp::acceptor m_acceptor;
};

} // namespace neurala
