//
// CountersServer.cpp
// ~~~~~~~~~~~~~~~~~~
//
// Source for the CountersServer class:
// - listens on a udp-v6 socket
// - forwards udp client requests to a CountersServerDispatcher
// - forwards back the replies from the CountersServerDispatcher to the clients
//
// This code is derived from the Boost tutorial here:
// https://www.boost.org/doc/libs/1_67_0/doc/html/boost_asio/tutorial/tutdaytime6/src.html
//
#include "CountersServer.h"
#include <iostream>
#include "Logger.h"

using boost::asio::ip::udp;

namespace ocs
{
namespace CountersServer
{

	// Ctor:
	// - Implements all the asio's server startup logic
	// - Invokes start_receive() before returning
	CountersServer::CountersServer(const Configuration& configuration, boost::asio::io_context& io_context, std::shared_ptr<CountersServerDispatcher> dispatcher)
	 : configuration_(configuration)
	 , socket_(io_context, udp::endpoint(udp::v6(), configuration.port))
	 , remote_endpoint_()
	 , recv_buffer_()
	 , dispatcher_(dispatcher)
	{
		start_receive();
	}
		
	// start_receive():
	// Prepares the server for asynchronous reception of client requests
	void CountersServer::start_receive()
	{
		socket_.async_receive_from(
			boost::asio::buffer(recv_buffer_), 
			remote_endpoint_,
			[this](boost::system::error_code ec,std::size_t recv_bytes)
			{ 
				handle_receive(ec,recv_bytes); 
			});
	}

	// handle_receive():
	// Handles the reception of a client request.
	// On a valid request:
	// - Forwards the request to the dispatcher for processing
	// - initiates the asynchronous sending of a response to the client
	// Otherwise, falls back to receiving state
	void CountersServer::handle_receive(const boost::system::error_code& ec,std::size_t recv_bytes)
	{
		if (!ec)
		{
			const auto reply = dispatcher_->dispatchCommand(recv_buffer_.cbegin(), recv_bytes);
			start_reply(reply);
		}
		else
		{
			Logger(warning) << "Received a request in error, ignored";
			start_receive();
		}
	}

	// start_reply():
	// Initiates the asynchronous sending of a response to a client
	void CountersServer::start_reply(const std::string& reply)
	{
		socket_.async_send_to(
			boost::asio::buffer(reply),
			remote_endpoint_,
			[this](boost::system::error_code error, std::size_t bytes_transferred) 
			{ 
				handle_send(error, bytes_transferred); 
			});
		
	}
		
	// handle_send():
	// Handles the completion of an asynchronous response sending
	// - prepares for processing another query with start_receive()
	void CountersServer::handle_send(const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/)
	{
		start_receive();
	}

} // namespace CountersServer
} // namespace ocs

