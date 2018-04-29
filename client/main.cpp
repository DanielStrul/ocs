//
// main.cpp
// ~~~~~~~~~~
//
// main for the counter server
//
// A good deal of the asio-related logic is derived from the Boost tutorial here:
// https://www.boost.org/doc/libs/1_67_0/doc/html/boost_asio/tutorial/tutdaytime4/src.html
//
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "Configuration.h"
#include "Logger.h"
#include "CountersClient.h"

namespace ocs
{
namespace CountersClient
{

	// Create a configuration container:
	// - Options are set from the command line options by parse_options() 
	// - The structure is then passed to all objects ctors
	Configuration configuration;

	// parse_options(argc, argv):
	// - Parses the command line options and stores them into the static Configuration object (configuration)
	// - If the options include '--help', prints help and returns +1
	// - Returns 0 otherwise
	int parse_options(int argc, char *argv[])
	{
		// A namespace alias is set for brievety
		namespace po = boost::program_options;
		
		// Define the supported options.
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce this help message")
			("host", po::value<>(&configuration.hostname),
				"set the name/ip of the target server (localhost by default)")
			("service", po::value<>(&configuration.service), 
				"set the udp port or service name on the target server (12345 by default)")
			("log-level", po::value<>(&configuration.minLogLevel),
				"set the log-level from -2 for trace to 3 for fatal (default, 0 for info)");
				
			
		// Parse the command line options, which are stored directly into the Configuration object
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
		
		// If the options include '--help', prints help and returns +1 to the caller
		if (vm.count("help")) 
		{
			std::cout << "Usage: client [options]\n";
			std::cout << desc << std::endl;
			return 1;
		}
		return 0;
	}


	// execute(argc, argv):
	// Client's main code, invoked directly from main()
	int execute(int argc, char *argv[])
	{
		// Parse command line options
		// If the client was launched with the option '--help', perform a clean exit to the shell
		int result = parse_options(argc, argv);
		if (result == 1)
			return 0;;

		try
		{
			// Display start-up banner and startup options
			Logger(info) << "=== client : starting ===";
			Logger(info) << "";
			Logger(info) << "Configuration:";
			Logger(info) << "\tTarget host:    " << configuration.hostname;
			Logger(info) << "\tTarget service: " << configuration.service;
			Logger(info) << "\tLog level: " 	 << configuration.minLogLevel;
			Logger(info) << "";

			// Set minimum log level
			Logger::setMinLevel(static_cast<LogLevel>(configuration.minLogLevel));
			
			// Create asio IO context
			boost::asio::io_context io_context;

			// Create a counters client object
			CountersClient service(configuration, io_context);
			
			// Pool the server every 5 seconds
			Logger(info) << "Pooling...";
			for (;;)
			{
				boost::asio::steady_timer t(io_context, boost::asio::chrono::seconds(5));
				service.getCounters();
				t.wait();
			}
			
			// Log shutdown
			Logger(info) << "=== server : shutdown ===";
			return 0;
		}
		catch (std::exception& e)
		{
			// On error, log the exception before shutting down
			Logger(fatal) << e.what();
			Logger(info) << "=== client : aborting ===";
			return -1;
		}
	}

} // namespace CountersClient
} // namespace ocs

// All the code for main() is moved to ocs::CountersClient::execute(),
// so as to be inside the client's namespace scope 
int main(int argc, char *argv[])
{
	return ocs::CountersClient::execute(argc, argv);
}
