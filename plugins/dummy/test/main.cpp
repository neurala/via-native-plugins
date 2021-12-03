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

#include <iostream>
#include <numeric>

#include "dummy.h"

int
main()
{
	using namespace neurala;
	plug::dummy::Discoverer dummyDiscoverer;

	// Discover the cameras exposed by this plugin
	const auto connectedCameras = dummyDiscoverer();
	CameraInfo dummyCameraInfo = connectedCameras[0];
	plug::dummy::Source dummyVideoSource(dummyCameraInfo);

	// Retrieve dummy metadata
	const dto::ImageMetadata metadata{dummyVideoSource.metadata()};
	std::cout << "Source metadata: " << metadata << std::endl;

	// Call dummy nextFrame
	if (!dummyVideoSource.nextFrame())
	{
		// Retrieve and print the image view's metadata
		const dto::ImageView view{dummyVideoSource.frame()};
		std::cout << "Image view metadata: " << view.metadata() << std::endl;
	}
}
