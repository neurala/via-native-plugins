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
