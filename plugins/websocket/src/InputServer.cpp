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

#include "InputServer.h"

#include <cstdint>
#include <numeric>
#include <string_view>
#include <vector>

#include <boost/json.hpp>

namespace neurala::plug::ws
{
InputServer::InputServer(const std::string_view ipAddress, const std::uint16_t port)
 : Server{ipAddress,
          port,
          {{"metadata",
            [&](WebSocketStream& stream, const boost::json::object&) { handleMetadata(stream); }},
           {"frame",
            [&](WebSocketStream& stream, const boost::json::object&) { handleFrame(stream); }}}},
   m_metadata{800, 600, "RGB", "planar", "uint8"}
{ }

void
InputServer::handleMetadata(WebSocketStream& stream)
{
	boost::json::object md;
	md["width"] = m_metadata.width;
	md["height"] = m_metadata.height;
	md["colorSpace"] = m_metadata.colorSpace.data();
	md["layout"] = m_metadata.layout.data();
	md["dataType"] = m_metadata.dataType.data();
	stream.write(net::buffer(serialize(md)));
}

void
InputServer::handleFrame(WebSocketStream& stream)
{
	std::vector<std::uint8_t> frameData(m_metadata.width * m_metadata.height
	                                    * m_metadata.colorSpace.size());
	static std::uint8_t init{}; // make every frame slightly different
	std::iota(begin(frameData), end(frameData), ++init);
	stream.write(net::buffer(frameData));
}

} // namespace neurala::plug::ws
