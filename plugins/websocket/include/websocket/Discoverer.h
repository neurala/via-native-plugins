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
/**
 * @brief Implementation of the CameraDiscoverer interface that provides connection information.
 */
class PLUGIN_API Discoverer final : public CameraDiscoverer
{
public:
	static void* create(PluginArguments&, PluginErrorCallback&) { return new Discoverer; }
	static void destroy(void* p) { delete reinterpret_cast<Discoverer*>(p); }

	/// Return information for the camera emulated by the plugin.
	[[nodiscard]] std::vector<CameraInfo> operator()() const noexcept final
	{
		return {{"0", "Input", "websocket", "127.0.0.1:54321"}};
	}
};

} // namespace neurala::plug::ws

#endif // NEURALA_STREAM_PLUGIN_DISCOVERER_H
