/*
 * Copyright (c) Neurala Inc. 2013-2021
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
	const ImageMetadata metadata{client.metadata()};
	BOOST_TEST(metadata.width() == 800);
	BOOST_TEST(metadata.height() == 600);
	BOOST_TEST(metadata.colorSpace() == EColorSpace::RGB);
	BOOST_TEST(metadata.layout() == EImageDataLayout::planar);
	BOOST_TEST(metadata.datatype() == EDatatype::uint8);
}

BOOST_AUTO_TEST_CASE(InsufficientBufferFrame)
{
	const ImageMetadata metadata{client.metadata()};
	BOOST_TEST(metadata.colorSpace() == EColorSpace::RGB);

	const auto& frameBuffer = client.frame();
	BOOST_CHECK_EQUAL(frameBuffer.size(), metadata.width() * metadata.height() * 3 - 1);
	BOOST_TEST(std::all_of(cbegin(frameBuffer), cend(frameBuffer), [](std::byte b) {
		return static_cast<char>(b) == 0;
	}));
}

BOOST_AUTO_TEST_CASE(SufficientBufferFrame)
{
	const ImageMetadata metadata{client.metadata()};
	BOOST_TEST(metadata.colorSpace() == EColorSpace::RGB);
	const auto& frameBuffer = client.frame();
	BOOST_CHECK_EQUAL(frameBuffer.size(), metadata.width() * metadata.height() * 3);
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
