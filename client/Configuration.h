#ifndef OCS_COUNTERS_CLIENT_CONFIGURATION_H
#define OCS_COUNTERS_CLIENT_CONFIGURATION_H
//
// Configuration.h
// ~~~~~~~~~~~~~~~
//
// Definition of the Configuration structure:
// - stores the server startup options (target host, target port...)
//

#include <string>
#include "Constants.h"

namespace ocs
{
namespace CountersClient
{

    // Configuration structure:
    // Container for the server startup options
    // No logic is required -> implemented as an open struct
    struct Configuration
    {
        // name (or ip) of the target server
        std::string hostname = "localhost";
        
        // port number or service name, "12345" by default
        std::string service = std::to_string(Constants::defaultPort);
        
        // minimum log level (info by default)
        int minLogLevel = 0;
    };

} // namespace CountersClient
} // namespace ocs

#endif // OCS_COUNTERS_CLIENT_CONFIGURATION_H
