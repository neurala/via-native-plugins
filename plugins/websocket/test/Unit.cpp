#define BOOST_TEST_MODULE Unit

#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>

#include "websocket/Client.h"
#include "websocket/Input.h"
#include "websocket/Output.h"
#include "websocket/Server.h"

using namespace neurala;

BOOST_AUTO_TEST_CASE(Metadata)
{
	boost::thread serverThread{[] { plug::ws::Server server{"127.0.0.1", 54321}; }};
	const ImageMetadata metadata{plug::ws::Client::get().metadata()};
	BOOST_TEST(metadata.width() == 800);
	BOOST_TEST(metadata.height() == 600);
	BOOST_TEST(metadata.colorSpace() == EColorSpace::RGB);
	BOOST_TEST(metadata.layout() == EImageDataLayout::planar);
	BOOST_TEST(metadata.datatype() == EDatatype::uint8);
}

BOOST_AUTO_TEST_CASE(Frame)
{
	BOOST_TEST(true);
}
