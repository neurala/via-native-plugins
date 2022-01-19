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

#include <iostream>

#include "EmptyResultsOutput.h"
#include "EmptyVideoSource.h"

int
main()
{
	using namespace neurala;
	plug::empty::VideoSource emptyVideoSource;
	plug::empty::ResultsOutput emptyResultsOutput;

	// Retrieve empty metadata
	const dto::ImageMetadata metadata{emptyVideoSource.metadata()};
	std::cout << "Metadata: " << metadata << std::endl;

	// Call empty nextFrame
	if (!emptyVideoSource.nextFrame())
	{
		// Retrieve and print the image view's metadata
		const dto::ImageView view{emptyVideoSource.frame()};
		std::cout << "Image view metadata: " << view.metadata() << std::endl;
	}

	// Trigger a fake output action
	emptyResultsOutput("{}", nullptr);
}
