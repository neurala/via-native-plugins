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

#ifndef NEURALA_PLUG_WS_INPUT_SERVER_H
#define NEURALA_PLUG_WS_INPUT_SERVER_H

#include <cstddef>
#include <cstdint>
#include <string_view>

#include <neurala/plugin/PluginBindings.h>

#include "Server.h"

namespace neurala::plug::ws
{
class PLUGIN_API InputServer final : public Server
{
public:
	InputServer(const std::string_view address, const std::uint16_t port);

private:
	/// Handle an image metadata request.
	void handleMetadata(WebSocketStream& stream);
	/// Handle a frame request.
	void handleFrame(WebSocketStream& stream);

	struct Metadata final
	{
		std::size_t width;
		std::size_t height;
		std::string_view colorSpace;
		std::string_view layout;
		std::string_view dataType;
	} m_metadata;
};

} // namespace neurala::plug::ws

#endif // NEURALA_PLUG_WS_INPUT_SERVER_H
