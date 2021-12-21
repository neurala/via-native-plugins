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

#include <boost/test/unit_test.hpp>

#include "websocket/Discoverer.h"

using namespace neurala;

BOOST_AUTO_TEST_SUITE(Discoverer)

BOOST_AUTO_TEST_CASE(GetCameraInfo)
{
	const std::vector<dto::CameraInfo> cameraInfo{plug::ws::Discoverer{}()};
	BOOST_TEST(cameraInfo.size() == 1);
	BOOST_TEST(cameraInfo.front().valid());
	BOOST_TEST(cameraInfo.front().id() == "0");
	BOOST_TEST(cameraInfo.front().name() == "websocket");
	BOOST_TEST(cameraInfo.front().type() == "Input");
	BOOST_TEST(cameraInfo.front().connection() == "127.0.0.1:54321");
}

BOOST_AUTO_TEST_SUITE_END()
