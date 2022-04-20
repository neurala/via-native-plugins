#include <cstdio>
#include <thread>

#include "Bridge.h"
#include "ResultOutput.h"
#include "VideoSource.h"

int main(int, char**)
{
	std::thread(neurala::dotnet::initialize).detach();

	neurala::CSharpVideoSource videoSource;
	neurala::CSharpResultOutput resultOutput;

	for (int i = 0; i < 5; ++i) {
		videoSource.nextFrame();

		const auto frame = videoSource.frame();

		videoSource.execute("(some action)");

		resultOutput("(image metadata goes here)", &frame);
	}

	return 0;
}
