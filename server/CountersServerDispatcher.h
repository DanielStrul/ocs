#ifndef OCS_COUNTERS_SERVER_COUNTERS_SERVER_DISPATCHER_H
#define OCS_COUNTERS_SERVER_COUNTERS_SERVER_DISPATCHER_H
//
// CountersServerDispatcher.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Header for the CountersServerDispatcher class:
// - receives client requests forwarded by a CountersServer
// - decodes and dispatches these requests to a CountersStore
// - receives the CountersStore's replies to theses requests
// - encodes the replies as messages
// - sends the messages to the CountersServer, which will forward them to the clients
//

#include <string>
#include "Configuration.h"
#include "CountersStore.h"

namespace ocs
{
namespace CountersServer
{

	// CountersServerDispatcher class:
	// - receives client requests forwarded by a CountersServer
	// - decodes and dispatches these requests to a CountersStore
	// - receives the CountersStore's replies to theses requests
	// - encodes the replies as messages
	// - sends the messages to the CountersServer, which will forward them to the clients
	class CountersServerDispatcher
	{
	public:
		// Ctor: 
		// Receives its dependencies from the caller, and stores them into internal variables
		CountersServerDispatcher(const Configuration& configuration, std::shared_ptr<CountersStore> store)
		: configuration_(configuration)
		, store_(store)
		{}
		
		// Dtor: 
		// releases shared resources (RAII)
		~CountersServerDispatcher() = default;

		// dispatchCommand(buffer, bytes):
		// Public API to be invoked by a CountersServer
		// - Executes the query processing workflow
		//   1) reception, decoding, dispatching of a requests to a CountersStore
		//   2) encoding and forwarding of the CountersStore's reply
		// - Encapsulate the workflow in a try-block so that exceptions when processing
		//	 queries should never bubble up to the server
		std::string dispatchCommand(const char* buffer, std::size_t bytes) const;
		
	private:
		// readCommand(buffer, bytes):
		// Private method invoked by dispatchCommand() when processing a command:
		// - reads the input buffer into a string
		// - removes any trailing newline
		std::string readCommand(const char* buffer, std::size_t bytes) const;
		
		// invokeExecutor(command):
		// Private method invoked by dispatchCommand() when processing a command:
		// - checks that the command corresponds to an expected command name ("GET")
		// - forwards the command to a method dedicated to this query (invoke_getCounters)
		// - formats the result ("OK:..." on success, "ERROR:..." on error)
		// - returns the formatted result to the caller (dispatchCommand)
		std::string invokeExecutor(const std::string& command) const;
		
		// invoke_getCounters(/*command*/):
		// Private method invoked by invokeExecutor() when processing a "GET" command:
		// - invokes the store's corresponding method
		// - converts the store's answer into a string
		std::string invoke_getCounters(const std::string& /*command*/) const;
		
		// formatResult(result):
		// Private method invoked by dispatchCommand() when processing a result
		// returned by invokeExecutor():
		// - prefixes the result with "OK:" for ease of error detection by the client
		std::string formatResult(const std::string& result) const;
		
		// formatError(exception):
		// Private method invoked by dispatchCommand() when processing an exception
		// raised during the processing of the query:
		// - prefixes the exception's message with "ERROR:" for ease of error detection by the client
		std::string formatError(const std::exception& e) const;
		
		// Internal logic
		const Configuration& 			configuration_;	// Startup configuration
		std::shared_ptr<CountersStore> 	store_;			// Counters's store
	};

} // namespace CountersServer
} // namespace ocs

#endif // OCS_COUNTERS_SERVER_COUNTERS_SERVER_DISPATCHER_H