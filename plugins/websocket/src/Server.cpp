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

#include "Server.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>

#include <boost/chrono.hpp>

namespace neurala::plug::ws
{
Server::Server(const std::string_view ipAddress,
               const std::uint16_t port,
               std::vector<std::pair<std::string_view, RequestHandler>>&& requestHandlers)
 : m_requestHandlers{},
   m_ioContext{1},
   m_acceptor{m_ioContext, tcp::endpoint{net::ip::make_address(ipAddress), port}},
   m_sessions{},
   m_running{true},
   m_thread{[&] { run(); }}
{
	for (auto& rh : requestHandlers)
	{
		m_requestHandlers.emplace(std::move(rh));
	}
}

Server::~Server()
{
	m_running = false;
	m_acceptor.close();
	for (boost::thread& session : m_sessions)
	{
		session.join();
	}
	m_thread.join();
}

void
Server::run()
{
	std::unique_ptr<volatile tcp::socket> socket;
	volatile bool detachingThread{};
	while (m_running)
	{
		while (detachingThread)
		{
			boost::this_thread::sleep(boost::posix_time::seconds(1));
		}
		socket = std::make_unique<volatile tcp::socket>(m_ioContext);
		m_acceptor.accept(const_cast<tcp::socket&>(*socket));
		detachingThread = true;
		boost::thread([&]() {
			session(std::move(const_cast<tcp::socket&>(*socket)));
			detachingThread = false;
		})
		 .detach();
	}
}

void
Server::session(tcp::socket&& socket)
{
	try
	{
		WebSocketStream stream{std::move(socket)};
		stream.binary(true);
		stream.set_option(beast::websocket::stream_base::decorator([](beast::websocket::response_type& res) {
			res.set(beast::http::field::server,
			        std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-sync");
		}));
		stream.accept();
		while (m_running)
		{
			handleRequest(stream);
		}
	}
	catch (const beast::system_error& se)
	{
		if (se.code() != beast::websocket::error::closed)
		{
			std::cerr << "Error: " << se.code().message() << '\n';
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
	}
}

void
Server::handleRequest(WebSocketStream& stream)
{
	beast::flat_buffer buffer;
	stream.read(buffer);
	const net::const_buffer readBuffer{buffer.cdata()};
	using namespace boost::json;
	value requestValue{
	 parse(string_view{reinterpret_cast<const char*>(readBuffer.data()), readBuffer.size()})};
	object& requestObject{requestValue.as_object()};
	const string& requestType{requestObject.at("request").as_string()};
	const RequestHandler& handler{
	 m_requestHandlers.at(std::string_view{requestType.data(), requestType.size()})};
	if (const auto requestBodyIt{requestObject.find("body")}; requestBodyIt != requestObject.cend())
	{
		handler(stream, requestBodyIt->value().as_object());
	}
	else
	{
		handler(stream, {});
	}
}

} // namespace neurala::plug::ws
