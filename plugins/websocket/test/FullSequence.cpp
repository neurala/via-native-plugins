/*
 * Copyright Neurala Inc. 2013-2024
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

#include <boost/test/unit_test.hpp>

#include "websocket/Input.h"
#include "websocket/Output.h"

using namespace neurala;

BOOST_AUTO_TEST_SUITE(FullSequence)

BOOST_AUTO_TEST_CASE(MultipleFrames)
{
	plug::ws::Input input;
	plug::ws::Output output;
	const dto::ImageMetadata imageMetadata{input.metadata()};
	for (std::size_t i{}; i < 10; ++i)
	{
		BOOST_TEST(input.nextFrame().value() == 0);
		const dto::ImageView imageView{input.frame()};
		output("{ \"status\": \"success\" }", nullptr);
	}
}

BOOST_AUTO_TEST_SUITE_END()
