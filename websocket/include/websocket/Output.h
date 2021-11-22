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

#ifndef NEURALA_PLUG_WS_OUTPUT_H
#define NEURALA_PLUG_WS_OUTPUT_H

#include <string>
#include <utility>

#include <boost/json.hpp>
#include <neurala/image/views/ImageView.h>
#include <neurala/plugin/PluginArguments.h>
#include <neurala/plugin/PluginBindings.h>
#include <neurala/plugin/PluginErrorCallback.h>
#include <neurala/utils/ResultsOutput.h>

#include "websocket/Client.h"

namespace neurala::websocket
{
/**
 * @brief Implementation of the ResultsOutput interface that handles resulting data.
 */
class PLUGIN_API Output final : public ResultsOutput
{
public:
	static void* create(PluginArguments&, PluginErrorCallback& ec)
	{
		try
		{
			return new Output;
		}
		catch (const std::system_error& se)
		{
			ec(se.code(), se.what());
		}
		catch (const std::exception& e)
		{
			ec(e.what());
		}
		catch (...)
		{
			ec("Could not create output interface");
		}

		return nullptr;
	}

	static void destroy(void* p) { delete reinterpret_cast<Output*>(p); }

	/**
	 * @brief Send a result JSON to the output server.
	 *
	 * @param metadata A JSON document containing information about the result.
	 * @param image A pointer to an image view, which may be null if no frame
	 *              is available or could be retrieved.
	 */
	void operator()(const std::string& metadata, const ImageView*) noexcept final
	{
		using namespace boost::json;
		m_client.sendResult(std::move(parse(string_view{metadata.data(), metadata.size()}).as_object()));
	}

private:
	Client m_client;
};

} // namespace neurala::websocket

#endif // NEURALA_PLUG_WS_OUTPUT_H
