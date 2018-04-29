#ifndef OCS_COUNTERS_SERVER_COUNTERS_SERVER_H
#define OCS_COUNTERS_SERVER_COUNTERS_SERVER_H
//
// CountersServer.h
// ~~~~~~~~~~~~~~~~~
//
// Header for the CountersServer class:
// - listens on a udp-v6 socket
// - forwards udp client requests to a CountersServerDispatcher
// - forwards back the replies from the CountersServerDispatcher to the clients
//

#include <array>
#include <string>
#include <boost/asio.hpp>
#include "Constants.h"
#include "CountersServerDispatcher.h"
#include "Configuration.h"

namespace ocs
{
namespace CountersServer
{

	// CountersServer class:
	// - listens on a udp-v6 socket
	// - forwards udp client requests to a CountersServerDispatcher
	// - forwards back the replies from the CountersServerDispatcher to the clients
	class CountersServer
	{
	public:
		// Ctor:
		// - Implements all the asio's server startup logic
		// - Invokes start_receive() before returning
		CountersServer(const Configuration& configuration, boost::asio::io_context& io_context, std::shared_ptr<CountersServerDispatcher> dispatcher);
		
	private:
		// start_receive():
		// Prepares the server for asynchronous reception of client requests
		void start_receive();
		
		// handle_receive():
		// Handles the reception of a client request.
		// On a valid request:
		// - Forwards the request to the dispatcher for processing
		// - initiates the asynchronous sending of a response to the client
		// Otherwise, falls back to receivinbg state
		void handle_receive(const boost::system::error_code& error, std::size_t recv_bytes);
		
		// start_reply():
		// Initiates the asynchronous sending of a response to a client
		void start_reply(const std::string& reply);

		// handle_send():
		// Handles the completion of an asynchronous response sending
		// - prepares for processing another query with start_receive()
		void handle_send(const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/);

		// Startup configuration parameters
		const Configuration& 							configuration_;
		
		// Variables used by asio logic
		boost::asio::ip::udp::socket 					socket_;
		boost::asio::ip::udp::endpoint 					remote_endpoint_;
		std::array<char, Constants::defaultBufferSize> 	recv_buffer_;
		
		// Dispatcher, decoding/encoding layer placed between the CountersServer and the CountersStore
		std::shared_ptr<CountersServerDispatcher> 		dispatcher_;
	};

} // namespace CountersServer
} // namespace ocs

#endif // OCS_COUNTERS_SERVER_COUNTERS_SERVER_H