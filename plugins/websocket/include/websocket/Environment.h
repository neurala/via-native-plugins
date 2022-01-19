/*
 * Copyright Neurala Inc. 2013-2022
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

#ifndef NEURALA_PLUG_WS_ENVIRONMENT_H
#define NEURALA_PLUG_WS_ENVIRONMENT_H

#include <cstdlib>

namespace neurala::plug::ws
{
inline const char* const envIpAddress{std::getenv("NEURALA_SERVER_IP_ADDRESS")};
inline const std::string_view ipAddress{envIpAddress == nullptr ? "127.0.0.1" : envIpAddress};

inline const char* const envPort{std::getenv("NEURALA_SERVER_PORT")};
inline const std::uint16_t port{
 static_cast<std::uint16_t>(envPort == nullptr ? 51234 : std::atoi(envPort))};

} // namespace neurala::plug::ws

#endif // NEURALA_PLUG_WS_ENVIRONMENT_H
