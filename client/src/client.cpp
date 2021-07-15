#include "client.h"

#include <cstdlib>
#include <iostream>
#include <unordered_map>

namespace neurala
{
/// Interpret an std::size_t value in a byte string starting at the given address.
static std::size_t
atoz(const char* const str)
{
	return static_cast<std::size_t>(std::atoll(str));
}

const std::unordered_map<std::string_view, EColorSpace> Client::colorSpaces{
 {"grayscale", EColorSpace::grayscale},
 {"bayerRG", EColorSpace::bayerRG},
 {"bayerGR", EColorSpace::bayerGR},
 {"bayerBG", EColorSpace::bayerBG},
 {"bayerGB", EColorSpace::bayerGB},
 {"RGB", EColorSpace::RGB},
 {"RGBA", EColorSpace::RGBA},
 {"BGR", EColorSpace::BGR},
 {"BGRA", EColorSpace::BGRA},
 {"RGB565", EColorSpace::RGB565},
 {"HSV", EColorSpace::HSV},
 {"YUV420", EColorSpace::YUV420},
 {"NV12", EColorSpace::NV12},
 {"NV21", EColorSpace::NV21}};

const std::unordered_map<std::string_view, EImageDataLayout> Client::imageDataLayouts{
 {"planar", EImageDataLayout::planar},
 {"interleaved", EImageDataLayout::interleaved},
 {"semiplanar", EImageDataLayout::semiplanar}};

const std::unordered_map<std::string_view, EDatatype> Client::imageDataTypes{
 {"boolean", EDatatype::boolean},
 {"uint8", EDatatype::uint8},
 {"uint16", EDatatype::uint16},
 {"binary16", EDatatype::binary16},
 {"binary32", EDatatype::binary32},
 {"binary64", EDatatype::binary64}};

ImageMetadata
Client::metadata()
{
	const net::const_buffer buffer{response("metadata")};
	const char* begin{reinterpret_cast<const char*>(buffer.data())};
	const char* const end{begin + buffer.size()};
	const std::size_t width{atoz(begin)};
	begin = parseMetadata(begin, end);
	const std::size_t height{atoz(begin)};
	begin = parseMetadata(begin, end);
	const EColorSpace colorSpace{colorSpaces.at(std::string_view{begin, metadataLength(begin, end)})};
	begin = parseMetadata(begin, end);
	const EImageDataLayout imageDataLayout{
	 imageDataLayouts.at(std::string_view{begin, metadataLength(begin, end)})};
	begin = parseMetadata(begin, end);
	const EDatatype dataType{imageDataTypes.at(std::string_view{begin, metadataLength(begin, end)})};
	return {width, height, colorSpace, imageDataLayout, dataType};
}

void
Client::frame(std::byte* const location)
{
	net::const_buffer buffer = response("frame");
	std::copy_n(reinterpret_cast<const std::byte*>(buffer.data()), buffer.size(), location);
}

Client::Client() : m_context{}, m_socket{m_context}, m_stream{m_socket}
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

net::const_buffer
Client::response(const std::string_view request)
{
	m_buffer.clear();
	m_stream.write(net::buffer(request));
	m_stream.read(m_buffer);
	const net::const_buffer data = m_buffer.cdata();
	if (data.size() < 1024)
	{
		std::cout << beast::make_printable(data) << '\n';
	}
	return data;
}

} // namespace neurala
