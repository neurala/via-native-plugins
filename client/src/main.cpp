#include "input.hpp"
#include "output.hpp"

int
main()
{
	using namespace neurala;
	Input input;
	Output output;
	const ImageMetadata metadata = input.metadata();
	const ImageView view = input.frame();
	output("{ \"result\": \"success\" }", nullptr);
}
