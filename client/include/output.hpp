#pragma once

#include <neurala/utils/ResultsOutput.h>

namespace neurala
{
class Output final : public ResultsOutput
{
public:
	static void* create(PluginArguments&, PluginErrorCallback&) { return new Output; }
	static void destroy(void* p) { delete reinterpret_cast<Output*>(p); }

	void operator()(const std::string& metadata, const ImageView*) final
	{
		Client::get().sendResult(metadata);
	}
};

} // namespace neurala
