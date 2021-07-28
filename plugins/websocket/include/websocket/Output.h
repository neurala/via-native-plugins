/*
 * Copyright Neurala Inc. 2013-2021. All rights reserved.
 *
 * Except as expressly permitted in the accompanying License Agreement, if at all, (a) you shall
 * not license, sell, rent, lease, transfer, assign, distribute, display, host, outsource, disclose
 * or otherwise commercially exploit or make this source code available to any third party; (b) you
 * shall not modify, make derivative works of, disassemble, reverse compile or reverse engineer any
 * part of the SDK; (c) You shall not access the SDK in order to build a similar or competitive
 * product or service; (d) no part of the this source may be copied, reproduced, distributed,
 * republished, downloaded, displayed, posted or transmitted in any form or by any means, including
 * but not limited to electronic, mechanical, photocopying, recording or other means; and (e) any
 * future release, update, or other addition to functionality of the SDK shall be subject to the
 * terms of the accompanying License Agreement. You must reproduce, on all copies made by you or
 * for you, and must not remove, alter, or obscure in any way all proprietary rights notices
 * (including copyright notices) of Neurala Inc or its suppliers on or within the copies of the
 * SDK. Any sample code provided with the SDK and designated as such are for illustrative purposes
 * only and are not to be included in your applications.
 *
 * In cases when the accompanying License Agreement permits redistribution of this file, the above
 * notice shall be reproduced its entirety in every copy of a distributed version of this file.
 */

#ifndef NEURALA_PLUG_WS_OUTPUT_H
#define NEURALA_PLUG_WS_OUTPUT_H

#include <string>

#include <boost/json.hpp>
#include <neurala/image/views/ImageView.h>
#include <neurala/plugin/PluginArguments.h>
#include <neurala/plugin/PluginBindings.h>
#include <neurala/plugin/PluginErrorCallback.h>
#include <neurala/utils/ResultsOutput.h>

#include "Client.h"

namespace neurala::plug::ws
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

	Output() : ResultsOutput{}, m_client{"127.0.0.1", 43210} { }

	/**
	 * @brief Send a result JSON to the output server.
	 *
	 * @param metadata A JSON document containing information about the result.
	 * @param image A pointer to an image view, which may be null if no frame
	 *              is available or could be retrieved.
	 */
	void operator()(const std::string& metadata, const ImageView*) final
	{
		using namespace boost::json;
		m_client.sendResult(parse(string_view{metadata.data(), metadata.size()}).as_object());
	}

private:
	Client m_client;
};

} // namespace neurala::plug::ws

#endif // NEURALA_PLUG_WS_OUTPUT_H
