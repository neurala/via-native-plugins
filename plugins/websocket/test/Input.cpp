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

#include <cstddef>
#include <vector>

#include <boost/test/unit_test.hpp>

#include "websocket/Input.h"

using namespace neurala;

BOOST_AUTO_TEST_SUITE(Input)

BOOST_AUTO_TEST_CASE(Metadata)
{
	const dto::ImageMetadata metadata{plug::ws::Input{}.metadata()};
	BOOST_TEST(metadata.datatype() == "uint8");
	BOOST_TEST(metadata.width() == 800);
	BOOST_TEST(metadata.height() == 600);
	BOOST_TEST(metadata.colorSpace() == "RGB");
	BOOST_TEST(metadata.layout() == "planar");
	BOOST_TEST(metadata.orientation() == "topLeft");
}

BOOST_AUTO_TEST_CASE(NextFrame)
{
	BOOST_TEST(plug::ws::Input{}.nextFrame().value() == 0);
}

BOOST_AUTO_TEST_CASE(ManagedFrame)
{
	plug::ws::Input input{};
	BOOST_TEST(input.nextFrame().value() == 0);
	const dto::ImageView view{input.frame()};
	BOOST_TEST(view.data() != nullptr);
	BOOST_TEST(view.width() == 800);
	BOOST_TEST(view.height() == 600);
	BOOST_TEST(view.datatype() == "uint8");
	BOOST_TEST(view.colorSpace() == "RGB");
	BOOST_TEST(view.layout() == "planar");
	BOOST_TEST(view.orientation() == "topLeft");
}

BOOST_AUTO_TEST_CASE(UnmanagedFrame)
{
	plug::ws::Input input{};
	BOOST_TEST(input.nextFrame().value() == 0);
	std::vector<std::byte> frameBuffer(800 * 600 * 3);
	const dto::ImageView view{input.frame(frameBuffer.data(), frameBuffer.size())};
	BOOST_TEST(view.data() == frameBuffer.data());
	BOOST_TEST(view.width() == 800);
	BOOST_TEST(view.height() == 600);
	BOOST_TEST(view.datatype() == "uint8");
	BOOST_TEST(view.colorSpace() == "RGB");
	BOOST_TEST(view.layout() == "planar");
	BOOST_TEST(view.orientation() == "topLeft");
}

BOOST_AUTO_TEST_SUITE_END()
