//
// main.cpp
// ~~~~~~~~~~
//
// main for the counter server
//
// A good deal of the asio-related logic is derived from the Boost tutorial here:
// https://www.boost.org/doc/libs/1_67_0/doc/html/boost_asio/tutorial/tutdaytime6/src.html
//
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "Logger.h"
#include "Configuration.h"
#include "CountersServerDispatcher.h"
#include "CountersServer.h"

namespace ocs
{
namespace CountersServer
{

    // Create a configuration container:
    // - Options are set from the command line options by parse_options() 
    // - The structure is then passed to all objects ctors
    static Configuration configuration;

    // parse_options(argc, argv):
    // - Parses the command line options and stores them into the static Configuration object (configuration)
    // - If the options include '--help', prints help and returns +1
    // - Returns 0 otherwise
    int parse_options(int argc, char *argv[])
    {
        // A namespace alias is set for brievety
        namespace po = boost::program_options;

        // Define the supported options
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce this help message")
            ("port", po::value<>(&configuration.port),
                "set the udp port on which to listen (default: 12345)")
            ("work-directory", po::value<>(&configuration.workDirectory),
                "set the work-directory for the persistent storage file (default: current directory)")
            ("log-level", po::value<>(&configuration.minLogLevel),
                "set the log-level from -2 for trace to 3 for fatal (default: 0 for info)");

        // Parse the command line options, which are stored directly into the Configuration object
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        // If the options include '--help', prints help and returns +1 to the caller
        if (vm.count("help")) 
        {
            std::cout << "Usage: server [options]\n";
            std::cout << desc << std::endl;
            return 1;
        }
        return 0;
    }

    // execute(argc, argv):
    // Server's main code, invoked directly from main()
    int execute(int argc, char *argv[])
    {
        // Parse command line options
        // If the server was launched with the option '--help', perform a clean exit to the shell
        int result = parse_options(argc, argv);
        if (result == 1)
            return 0;;

        try
        {
            // Display start-up banner and startup options
            Logger(info) << "=== server : starting ===";
            Logger(info) << "";
            Logger(info) << "Configuration:";
            Logger(info) << "\tListen port:    " << configuration.port;
            Logger(info) << "\tWork directory: " << configuration.workDirectory;
            Logger(info) << "\tLog level: "      << configuration.minLogLevel;
            Logger(info) << "";

            // Set minimum log level
            Logger::setMinLevel(static_cast<LogLevel>(configuration.minLogLevel));

            // Create asio IO context
            boost::asio::io_service io_context;

            // Set handler for graceful shutdown.
            boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
            signals.async_wait(
                [&io_context](boost::system::error_code /* ec */, int signal_number)
                {
                    Logger(info) << "Received signal " << signal_number;
                    io_context.stop(); 
                }
            );


            // Create a counters store
            std::shared_ptr<CountersStore> store(new CountersStore(configuration));

            // Attach a dispatcher to the store, and create a counters server object
            std::shared_ptr<CountersServerDispatcher> dispatcher(new CountersServerDispatcher(configuration, store));
            CountersServer server(configuration, io_context, dispatcher);

            // Run the server
            Logger(info) << "Listening...";
            io_context.run();

            // Log shutdown
            Logger(info) << "=== server : shutdown ===";
            return 0;
        }
        catch (std::exception& e)
        {
            // On error, log the exception before shutting down
            Logger(fatal) << e.what();
            Logger(info) << "=== server : aborting ===";
            return -1;
        }
    }

} // namespace CountersServer
} // namespace ocs

// All the code for main() is moved to ocs::CountersServer::execute(),
// so as to be inside the server's namespace scope 
int main(int argc, char *argv[])
{
    return ocs::CountersServer::execute(argc, argv);
}
