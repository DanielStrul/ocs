#ifndef OCS_COUNTERS_CLIENT_COUNTERS_CLIENT_H
#define OCS_COUNTERS_CLIENT_COUNTERS_CLIENT_H
//
// CountersClient.h
// ~~~~~~~~~~~~~~~~~
//
// Header for the CountersClient class:
// - opens an udp-v6 socket
// - provides an API for:
//		sending a request to a counters server
// 		receiving and displaying the server's reply
//

#include <boost/asio.hpp>
#include "Configuration.h"

namespace ocs
{
namespace CountersClient
{

	// CountersClient class:
	// - opens an udp-v6 socket
	// - provides an API for:
	//		sending a request to a counters server
	// 		receiving and displaying the server's reply
	class CountersClient
	{
	public:
		// Ctor:
		// Implements the asio's server startup logic
		CountersClient(const Configuration& configuration, boost::asio::io_context& io_context);
		
		// getCounters():
		// - sends a request to the target server
		// - receives the server's reply (message)
		// - decode the reply (message) into a query count
		// - display the count to the console (via the logger)
		// - encapsulate the whole workflow in a try-block so that exceptions should not bubble-up to the main polling loop
		void getCounters();
		
	private:
		// sendCommand():
		// Sends a "GET" command to the target server
		void sendCommand();
		
		// receiveReply():
		// Receives a reply to a command from the target server
		std::string receiveReply();
		
		// decodeCount():
		// - decodes a "GET" reply message into a query count
		// - throws if the reply is an error message or cannot be read
		unsigned long long decodeCount(const std::string& reply);
		
	private:
		const Configuration& 			configuration_;
		boost::asio::io_context& 		io_context_;
		boost::asio::ip::udp::socket 	socket_;
		boost::asio::ip::udp::endpoint	receiver_endpoint_;
	};

} // namespace CountersClient
} // namespace ocs

#endif // OCS_COUNTERS_CLIENT_COUNTERS_CLIENT_H