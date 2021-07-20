#define BOOST_TEST_MODULE Client

#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>

#include "websocket/Client.h"
#include "websocket/Input.h"
#include "websocket/Output.h"
#include "websocket/Server.h"

using namespace neurala;

struct ServerFixture final
{
	boost::thread serverThread{[] { plug::ws::Server server{"127.0.0.1", 54321}; }};
};

BOOST_TEST_GLOBAL_FIXTURE(ServerFixture);

BOOST_AUTO_TEST_CASE(Metadata)
{
	const ImageMetadata metadata{plug::ws::Client::get().metadata()};
	BOOST_TEST(metadata.width() == 800);
	BOOST_TEST(metadata.height() == 600);
	BOOST_TEST(metadata.colorSpace() == EColorSpace::RGB);
	BOOST_TEST(metadata.layout() == EImageDataLayout::planar);
	BOOST_TEST(metadata.datatype() == EDatatype::uint8);
}

BOOST_AUTO_TEST_CASE(InsufficientBufferFrame)
{
	const ImageMetadata metadata{plug::ws::Client::get().metadata()};
	BOOST_TEST(metadata.colorSpace() == EColorSpace::RGB);
	std::vector<std::byte> frameBuffer(metadata.width() * metadata.height() * 3 - 1);
	BOOST_TEST(!plug::ws::Client::get().frame(frameBuffer.data(), frameBuffer.size()));
	BOOST_TEST(std::all_of(cbegin(frameBuffer), cend(frameBuffer), [](std::byte b) {
		return static_cast<char>(b) == 0;
	}));
}

BOOST_AUTO_TEST_CASE(SufficientBufferFrame)
{
	const ImageMetadata metadata{plug::ws::Client::get().metadata()};
	BOOST_TEST(metadata.colorSpace() == EColorSpace::RGB);
	std::vector<std::byte> frameBuffer(metadata.width() * metadata.height() * 3);
	BOOST_TEST(plug::ws::Client::get().frame(frameBuffer.data(), frameBuffer.size()));
}

BOOST_AUTO_TEST_CASE(Response)
{
	plug::ws::Client::get().sendResult("{ \"result\": \"success\" }");
}
