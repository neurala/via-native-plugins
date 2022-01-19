/*
 * Copyright Neurala Inc. 2013-2022
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

#include <algorithm>
#include <cstddef>
#include <vector>

#include <boost/json.hpp>
#include <boost/test/unit_test.hpp>

#include "websocket/Client.h"

using namespace neurala;

struct ClientFixture
{
	plug::ws::Client client;
};

BOOST_FIXTURE_TEST_SUITE(Client, ClientFixture)

BOOST_AUTO_TEST_CASE(Metadata)
{
	const dto::ImageMetadata metadata{client.metadata()};
	BOOST_TEST(metadata.width() == 800);
	BOOST_TEST(metadata.height() == 600);
	BOOST_TEST(metadata.colorSpace() == "RGB");
	BOOST_TEST(metadata.layout() == "planar");
	BOOST_TEST(metadata.datatype() == "uint8");
}

BOOST_AUTO_TEST_CASE(Frame)
{
	BOOST_TEST(client.nextFrame().value() == 0);
	const dto::ImageView frame{client.frame()};
	BOOST_TEST(frame.colorSpace() == "RGB");
	BOOST_CHECK_EQUAL(client.frameSize(), frame.width() * frame.height() * 3);
}

BOOST_AUTO_TEST_CASE(Response)
{
	try
	{
		client.sendResult(boost::json::object{{"status", "success"}});
	}
	catch (...)
	{
		BOOST_FAIL("Caught exception while sending result.");
	}
}

BOOST_AUTO_TEST_SUITE_END()
