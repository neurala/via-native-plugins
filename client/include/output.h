#ifndef NEURALA_STREAM_PLUGIN_OUTPUT_H
#define NEURALA_STREAM_PLUGIN_OUTPUT_H

#include <string>

#include <neurala/image/views/ImageView.h>
#include <neurala/plugin/detail/PluginArguments.h>
#include <neurala/plugin/detail/PluginManager.h>
#include <neurala/utils/ResultsOutput.h>

#include "client.h"

namespace neurala
{
class Output final : public ResultsOutput
{
public:
	static void* create(PluginArguments&, PluginErrorCallback&) { return new Output; }
	static void destroy(void* p) { delete reinterpret_cast<Output*>(p); }

	/**
	 * @brief Function call operator for invoking the output action.
	 *
	 * @param metadata A JSON document containing information about the result.
	 * @param image A pointer to an image view, which may be null if no frame
	 *              is available or could be retrieved.
	 */
	void operator()(const std::string& metadata, const ImageView*) final
	{
		Client::get().sendResult(metadata);
	}
};

} // namespace neurala

#endif // NEURALA_STREAM_PLUGIN_OUTPUT_H
