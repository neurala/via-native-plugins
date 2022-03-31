#include <cstdio>

#include "ResultOutput.h"
#include "VideoSource.h"

int main(int, char**)
{
	neurala::CSharpVideoSource videoSource;
	neurala::CSharpResultOutput resultOutput;

	videoSource.nextFrame();

	const auto frame = videoSource.frame();

	printf("Got %dx%d image.\n", frame.width(), frame.height());

	videoSource.execute("(some action)");

	resultOutput("(image metadata goes here)", &frame);

	return 0;
}
