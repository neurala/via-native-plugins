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

/**
 * @brief Websocket client that receives input frames from a local server.
 */
class Client final
{
public:
	/// Return the client instance.
	static Client& get()
	{
		static Client instance;
		return instance;
	}

	/// Retrieve metadata regarding frames fed by the server.
	ImageMetadata metadata();

	/// Retrieve the next frame and copy data to the specified location.
	void frame(std::byte* const location);

	/// Send the result of processing a frame back to the server.
	void sendResult(const std::string& result) { response(result); }

private:
	/// Return a pointer to the start of the next metadata element.
	static const char* parseMetadata(const char* const begin, const char* const end)
	{
		return std::find(begin, end, ';') + 1;
	}

	/// Return the length of the current metadata element.
	static std::size_t metadataLength(const char* const begin, const char* const end)
	{
		return std::find(begin, end, ';') - begin;
	}

	static constexpr std::string_view ip_address{"127.0.0.1"};
	static constexpr std::uint16_t port{54321};

	static const std::unordered_map<std::string_view, EColorSpace> colorSpaces;
	static const std::unordered_map<std::string_view, EImageDataLayout> imageDataLayouts;
	static const std::unordered_map<std::string_view, EDatatype> imageDataTypes;

	Client();
	Client(const Client&) = delete;
	Client(Client&&) = delete;
	Client& operator=(const Client&) = delete;
	Client& operator=(Client&&) = delete;
	~Client() { m_stream.close(beast::websocket::close_code::normal); }

	/// Retrieve the response for a given request.
	net::const_buffer response(const std::string_view request);

	net::io_context m_context;
	tcp::socket m_socket;
	beast::websocket::stream<tcp::socket&> m_stream;
	beast::flat_buffer m_buffer;
};

} // namespace neurala
