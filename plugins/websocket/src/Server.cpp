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
			boost::this_thread::sleep(boost::chrono::milliseconds(1000));
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
