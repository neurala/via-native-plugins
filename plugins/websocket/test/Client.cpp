/*
 * Copyright Neurala Inc. 2013-2021. All rights reserved.
 *
 * Except as expressly permitted in the accompanying License Agreement, if at all, (a) you shall
 * not license, sell, rent, lease, transfer, assign, distribute, display, host, outsource, disclose
 * or otherwise commercially exploit or make this source code available to any third party; (b) you
 * shall not modify, make derivative works of, disassemble, reverse compile or reverse engineer any
 * part of the SDK; (c) You shall not access the SDK in order to build a similar or competitive
 * product or service; (d) no part of the this source may be copied, reproduced, distributed,
 * republished, downloaded, displayed, posted or transmitted in any form or by any means, including
 * but not limited to electronic, mechanical, photocopying, recording or other means; and (e) any
 * future release, update, or other addition to functionality of the SDK shall be subject to the
 * terms of the accompanying License Agreement. You must reproduce, on all copies made by you or
 * for you, and must not remove, alter, or obscure in any way all proprietary rights notices
 * (including copyright notices) of Neurala Inc or its suppliers on or within the copies of the
 * SDK. Any sample code provided with the SDK and designated as such are for illustrative purposes
 * only and are not to be included in your applications.
 *
 * In cases when the accompanying License Agreement permits redistribution of this file, the above
 * notice shall be reproduced its entirety in every copy of a distributed version of this file.
 */

#include <boost/test/unit_test.hpp>

#include "websocket/Client.h"

using namespace neurala;

struct ClientFixture
{
	plug::ws::Client client{"127.0.0.1", 54321};
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
	std::vector<std::byte> frameBuffer(metadata.width() * metadata.height() * 3 - 1);
	BOOST_TEST(!client.frame(frameBuffer.data(), frameBuffer.size()));
	BOOST_TEST(std::all_of(cbegin(frameBuffer), cend(frameBuffer), [](std::byte b) {
		return static_cast<char>(b) == 0;
	}));
}

BOOST_AUTO_TEST_CASE(SufficientBufferFrame)
{
	const ImageMetadata metadata{client.metadata()};
	BOOST_TEST(metadata.colorSpace() == EColorSpace::RGB);
	std::vector<std::byte> frameBuffer(metadata.width() * metadata.height() * 3);
	BOOST_TEST(client.frame(frameBuffer.data(), frameBuffer.size()));
}

BOOST_AUTO_TEST_CASE(Response)
{
	try
	{
		plug::ws::Client outputClient{"127.0.0.1", 43210};
		outputClient.sendResult("{ \"result\": \"success\" }");
	}
	catch (...)
	{
		BOOST_FAIL("Caught exception while sending result.");
	}
}

BOOST_AUTO_TEST_SUITE_END()
