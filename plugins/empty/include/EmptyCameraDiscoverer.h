/*
 * Copyright Neurala Inc. 2013-2024
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

#ifndef NEURALA_EMPTY_CAMERA_DISCOVERER_H
#define NEURALA_EMPTY_CAMERA_DISCOVERER_H

#include <vector>

#include <neurala/plugin/PluginArguments.h>
#include <neurala/plugin/PluginBindings.h>
#include <neurala/plugin/PluginErrorCallback.h>

#include <neurala/video/CameraDiscoverer.h>
#include <neurala/video/dto/CameraInfo.h>

namespace neurala::plug::empty
{
class CameraDiscoverer final : public neurala::CameraDiscoverer
{
public:
	static void* create(PluginArguments&, PluginErrorCallback&) { return new CameraDiscoverer; }
	static void destroy(void* p) { delete reinterpret_cast<CameraDiscoverer*>(p); }

	/// Return information for the camera emulated by the plugin.
	[[nodiscard]] std::vector<dto::CameraInfo> operator()() const noexcept final { return {}; }
};

} // namespace neurala::plug::empty

#endif // NEURALA_EMPTY_CAMERA_DISCOVERER_H
