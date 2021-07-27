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

#ifndef NEURALA_PLUG_WS_INPUT_H
#define NEURALA_PLUG_WS_INPUT_H

#include <cstddef>
#include <optional>
#include <string>
#include <system_error>
#include <vector>

#include <neurala/plugin/PluginArguments.h>
#include <neurala/plugin/PluginBindings.h>
#include <neurala/plugin/PluginErrorCallback.h>
#include <neurala/video/VideoSource.h>

#include "Client.h"

namespace neurala::plug::ws
{
/**
 * @brief Implementation of the VideoSource interface that retrieves frame data.
 */
class PLUGIN_API Input final : public VideoSource
{
public:
	static void* create(PluginArguments& args, PluginErrorCallback& ec)
	{
		const std::string& connection{args.get<0, const CameraInfo>().connection()};
		const std::size_t delimiterIndex = connection.find(':');

		try
		{
			return new Input{std::string_view{connection.data(), delimiterIndex},
			                 static_cast<std::uint16_t>(std::atoi(connection.data() + delimiterIndex + 1))};
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
			ec("Could not create input interface");
		}

		return nullptr;
	}

	static void destroy(void* p) { delete reinterpret_cast<Input*>(p); }

	/**
	 * @param ipAddress server's IP address
	 * @param port server's listening port
	 */
	Input(const std::string_view ipAddress, const std::uint16_t port);

	// Image dimension information
	[[nodiscard]] ImageMetadata metadata() const final { return cachedMetadata(); }

	// Query new frames​
	[[nodiscard]] std::error_code nextFrame() final;

	// Get a frame from host memory, data needs to be valid until the end of processing​
	[[nodiscard]] ImageView frame() final { return {cachedMetadata(), m_frame.data()}; }

	// Copy a frame into the buffer provided as argument
	[[nodiscard]] ImageView frame(std::byte* data, std::size_t size) final;

	// Executes an arbitrary action on the video source
	[[nodiscard]] std::error_code execute(const std::string&) final { return {}; }

private:
	// Access the image metadata. Only retrieve over the network if necessary.
	const ImageMetadata& cachedMetadata() const;

	mutable Client m_client;
	mutable std::optional<ImageMetadata> m_metadata;
	std::vector<std::byte> m_frame;
};

} // namespace neurala::plug::ws

#endif // NEURALA_PLUG_WS_INPUT_H
