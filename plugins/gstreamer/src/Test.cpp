/*
 * Copyright Neurala Inc. 2013-2023
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

#include <gst/gst.h>

#include "GStreamerVideoSource.h"

int main()
{
	char pipelineVariable[] = "NEURALA_GSTREAMER_PIPELINE=playbin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm video-sink=\"videoconvert ! video/x-raw,format=RGB,width=320,height=240 ! appsink name=neurala_appsink\"";
	char widthVariable[] = "NEURALA_GSTREAMER_WIDTH=320";
	char heightVariable[] = "NEURALA_GSTREAMER_HEIGHT=240";

	putenv(pipelineVariable);
	putenv(widthVariable);
	putenv(heightVariable);
	gst_init(0, nullptr);

	neurala::GStreamerVideoSource videoSource;

	for (auto i = 0; i < 16; ++i)
	{
		const auto status = videoSource.nextFrame();

		if (status)
		{
			const auto code = status.value();
			printf("%d\n", code);
			break;
		}

		const auto frame = videoSource.frame();
		const auto width = frame.width();
		const auto height = frame.height();
		const auto firstByte = frame.dataAs<uint8_t>()[0];

		printf("%zu %zu %u\n", width, height, firstByte);
	}

	return 0;
}
