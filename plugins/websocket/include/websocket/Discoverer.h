#ifndef NEURALA_PLUG_WS_DISCOVERER_H
#define NEURALA_PLUG_WS_DISCOVERER_H

#include <vector>

#include <neurala/plugin/PluginArguments.h>
#include <neurala/plugin/PluginBindings.h>
#include <neurala/plugin/PluginErrorCallback.h>
#include <neurala/video/CameraDiscoverer.h>
#include <neurala/video/CameraInfo.h>

namespace neurala::plug::ws
{
class PLUGIN_API Discoverer final : public CameraDiscoverer
{
public:
	static void* create(PluginArguments&, PluginErrorCallback&) { return new Discoverer; }
	static void destroy(void* p) { delete reinterpret_cast<Discoverer*>(p); }

	// Scan for all available cameras
	[[nodiscard]] std::vector<CameraInfo> operator()() const final
	{
		return {{"0", "Input", "websocket", "127.0.0.1:54321"}};
	}
};

} // namespace neurala::plug::ws

#endif // NEURALA_STREAM_PLUGIN_DISCOVERER_H
