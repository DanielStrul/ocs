#ifndef OCS_COUNTERS_SERVER_CONFIGURATION_H
#define OCS_COUNTERS_SERVER_CONFIGURATION_H
//
// Configuration.h
// ~~~~~~~~~~~~~~~
//
// Definition of the Configuration structure:
// - stores the server startup options (listen port, work directory...)
//

#include <string>
#include "Constants.h"

namespace ocs
{
namespace CountersServer
{

    // Configuration structure:
    // Container for the server startup options
    // No logic is required -> implemented as an open struct
    struct Configuration
    {
        // listen port, 12345 by default
        int port = Constants::defaultPort;
        
        // Work directory for storing runtime files
        std::string workDirectory = ".";
        
        // minimum log level (info by default)
        int minLogLevel = 0;
    };

} // namespace CountersServer
} // namespace ocs

#endif // OCS_COUNTERS_SERVER_CONFIGURATION_H
