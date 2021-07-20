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

#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/thread.hpp>

namespace neurala::plug::ws
{
Server::Server(const std::string_view address, const std::uint16_t port)
 : m_ioContext{1},
   m_acceptor{m_ioContext, tcp::endpoint{net::ip::make_address(address), port}},
   m_metadata{800, 600, "RGB", "planar", "uint8"}
{
	run();
}

void
Server::run()
{
	std::unique_ptr<volatile tcp::socket> socket{std::make_unique<volatile tcp::socket>(m_ioContext)};
	volatile bool detachingThread{};
	while (true)
	{
		while (detachingThread)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
		beast::websocket::stream<tcp::socket> stream{std::move(socket)};
		stream.binary(true);
		stream.set_option(beast::websocket::stream_base::decorator([](beast::websocket::response_type& res) {
			res.set(beast::http::field::server,
			        std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-sync");
		}));
		stream.accept();
		while (true)
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
Server::handleRequest(beast::websocket::stream<tcp::socket>& stream)
{
	beast::flat_buffer buffer;
	stream.read(buffer);
	using WebSocketStream = beast::websocket::stream<tcp::socket>;
	static const std::unordered_map<std::string_view, std::function<void(WebSocketStream&)>> handlers{
	 {"metadata", [&](WebSocketStream& stream) { handleMetadata(stream); }},
	 {"frame", [&](WebSocketStream& stream) { handleFrame(stream); }},
	 {"{ \"result\": \"success\" }", [&](WebSocketStream& stream) { handleResult(stream); }},
	};
	const net::const_buffer key{buffer.cdata()};
	handlers.at(std::string_view(reinterpret_cast<const char*>(key.data()), key.size()))(stream);
}

void
Server::handleMetadata(beast::websocket::stream<tcp::socket>& stream)
{
	std::string metadata;
	const auto add{[&](const std::string_view element) {
		metadata += element;
		metadata += ';';
	}};
	add(std::to_string(m_metadata.width));
	add(std::to_string(m_metadata.height));
	add(m_metadata.colorSpace);
	add(m_metadata.layout);
	add(m_metadata.dataType);
	stream.write(net::buffer(metadata));
}

void
Server::handleFrame(beast::websocket::stream<tcp::socket>& stream)
{
	std::vector<std::uint8_t> frameData(m_metadata.width * m_metadata.height
	                                    * m_metadata.colorSpace.size());
	std::iota(begin(frameData), end(frameData), 0);
	stream.write(net::buffer(frameData));
}

void
Server::handleResult(beast::websocket::stream<tcp::socket>& stream)
{
	stream.write(net::buffer("DONE"));
}

} // namespace neurala::plug::ws
