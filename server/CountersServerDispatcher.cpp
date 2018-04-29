//
// CountersServerDispatcher.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source for the CountersServerDispatcher class:
// - receives client requests forwarded by a CountersServer
// - decodes and dispatches these requests to a CountersStore
// - receives the CountersStore's replies to theses requests
// - encodes the replies as messages
// - sends the messages to the CountersServer, which will forward them to the clients
//
#include "CountersServerDispatcher.h"
#include "Logger.h"

namespace ocs
{
namespace CountersServer
{

    // dispatchCommand(buffer, bytes)
    // Public API to be invoked by a CountersServer
    // - Executes the query processing workflow
    //   1) reception, decoding, dispatching of a requests to a CountersStore
    //   2) encoding and forwarding of the CountersStore's reply
    // - Encapsulate the workflow in a try-block so that exceptions when processing
    //   queries should never bubble up to the server
    std::string CountersServerDispatcher::dispatchCommand(const char* buffer, std::size_t bytes) const
    {
        try
        {
            const auto command = readCommand(buffer, bytes);
            Logger(debug) << "Received a command '" << command << "', dispatching";
            
            const auto result = invokeExecutor(command);
            
            Logger(debug) << "Command was successfully processed, result= " << result;
            return formatResult(result);
        }
        catch (std::exception& e)
        {
            Logger(error) << e.what();
            return formatError(e);
        }
    }

    
    // readCommand(buffer, bytes):
    // Private method invoked by dispatchCommand() when processing a command:
    // - reads the input buffer into a string
    // - removes any trailing newline
    std::string CountersServerDispatcher::readCommand(const char* buffer, std::size_t bytes) const
    {
        if (bytes && buffer[bytes-1] == '\n')
            --bytes;
        return std::string(buffer, bytes);
    }

    
    // invokeExecutor(command):
    // Private method invoked by dispatchCommand() when processing a command:
    // - checks that the command corresponds to an expected command name ("GET")
    // - forwards the command to a method dedicated to this query (invoke_getCounters)
    // - formats the result ("OK:..." on success, "ERROR:..." on error)
    // - returns the formatted result to the caller (dispatchCommand)
    std::string CountersServerDispatcher::invokeExecutor(const std::string& command) const
    {
        // Only one command for now, so a simple if-block is used (faster than a LUT search)
        if (command == "GET")
            return invoke_getCounters(command);
        
        // Throw if the command is not valid: 
        // dispatchCommand() will convert the exception into an error message
        const auto msg = "Unrecognized command: '" + command + "'";
        Logger(error) << msg;
        throw std::logic_error(msg);
    }

    
    // invoke_getCounters(/*command*/):
    // Private method invoked by invokeExecutor() when processing a "GET" command:
    // - invokes the store's corresponding method
    // - converts the store's answer into a string
    std::string CountersServerDispatcher::invoke_getCounters(const std::string& /*command*/) const
    {
        const auto result = store_->getCounters();
        return std::to_string(result);
    }

    
    // formatResult(result):
    // Private method invoked by dispatchCommand() when processing a result
    // returned by invokeExecutor():
    // - prefixes the result with "OK:" for ease of error detection by the client
    std::string CountersServerDispatcher::formatResult(const std::string& result) const
    {
        return "OK: " + result + "\n";
    }

    
    // formatError(exception):
    // Private method invoked by dispatchCommand() when processing an exception
    // raised during the processing of the query:
    // - prefixes the exception's message with "ERROR:" for ease of error detection by the client
    std::string CountersServerDispatcher::formatError(const std::exception& e) const
    {
        return std::string("ERROR: ") + e.what() + "\n";
    }

} // namespace CountersServer
} // namespace ocs
