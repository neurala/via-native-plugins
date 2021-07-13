#pragma once

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string_view>


#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/config.hpp>


namespace neurala
{
namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class Client final
{
public:
	static Client& get()
	{
		static Client instance;
		return instance;
	}

	ImageMetadata metadata()
	{
		const net::const_buffer buffer = response("metadata");
		return {2560, 1440, EColorSpace::RGBA, EImageDataLayout::planar, EDatatype::uint8};
	}

	void frame(std::byte* const location)
	{
		std::cerr << "SENDING REQUEST" << std::endl;
		net::const_buffer buffer = response("frame");
		std::cerr << "GOT BUFFER" << std::endl;
		std::copy_n(reinterpret_cast<const std::byte*>(buffer.data()), buffer.size(), location);
		std::cerr << "COPIED BUFFER" << std::endl;
	}

	void sendResult(const std::string& result) { response(result); }

private:
	static constexpr std::string_view ip_address{"127.0.0.1"};
	static constexpr std::uint16_t port{54321};

	Client() : m_context{}, m_socket{m_context}, m_stream{m_socket}
	{
		boost::system::error_code ec;
		tcp::endpoint endpoint{net::ip::make_address(ip_address, ec), port};
		m_socket.connect(endpoint, ec);
		if (ec)
		{
			std::cerr << "Failed to connect: " << ec.message() << ".\n";
			std::exit(1);
		}
		std::cout << "Client connected.\n";
		m_stream.handshake(ip_address.data(), "/");
	}

	~Client() { m_stream.close(beast::websocket::close_code::normal); }

	net::const_buffer response(const std::string_view request)
	{
		std::cerr << "MAX SIZE: " << m_buffer.max_size() << std::endl;
		m_buffer.clear();
		m_stream.write(net::buffer(request));
		std::cout << "SENT REQUEST " << request << std::endl;
		std::cout << "CREATED BUFFER\n";
		m_stream.read(m_buffer);
		std::cout << "READ BUFFER\n";
		const net::const_buffer data = m_buffer.cdata();
		std::cout << "GOT DATA\n";
		std::cout << data.size() << std::endl;
		// std::cout << beast::make_printable(data) << std::endl;
		return data;
	}

	net::io_context m_context;
	tcp::socket m_socket;
	beast::websocket::stream<tcp::socket&> m_stream;
	beast::flat_buffer m_buffer;
};

} // namespace neurala
