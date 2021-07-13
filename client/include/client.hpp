#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/config.hpp>
#include <neurala/image/ImageMetadata.h>

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

	ImageMetadata metadata();

	void frame(std::byte* const location);

	void sendResult(const std::string& result) { response(result); }

private:
	static constexpr std::string_view ip_address{"127.0.0.1"};
	static constexpr std::uint16_t port{54321};

	static const char* parseMetadata(const char* const begin, const char* const end)
	{
		return std::find(begin, end, ';') + 1;
	}

	static std::size_t metadataLength(const char* const begin, const char* const end)
	{
		return std::find(begin, end, ';') - begin;
	}

	Client();

	~Client() { m_stream.close(beast::websocket::close_code::normal); }

	net::const_buffer response(const std::string_view request);

	net::io_context m_context;
	tcp::socket m_socket;
	beast::websocket::stream<tcp::socket&> m_stream;
	beast::flat_buffer m_buffer;
};

} // namespace neurala
