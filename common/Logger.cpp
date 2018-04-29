//
// Logger.cpp
// ~~~~~~~~~~
//
// Source for the Logger class, which serves two purposes:
// In general: is meant to provide a thin wrapper around some concrete logger facility or library
// Meant to allow for changing the logger (for e.g. Boost or Google) without too much 
// hassle in the client code
// In my case: is meant to provide me with a logger despite the fact that the boost logger 
// is not willing to compile on my box ;-)
//

#include "Logger.h"
#include <iostream>

namespace ocs
{

    // Static map of log-level names
    const std::unordered_map<LogLevel,const std::string> Logger::logLevelTexts =
    {
        {trace,"trace"},
        {debug,"debug"},
        {info,"info"},
        {warning,"warning"},
        {error,"error"},
        {fatal,"fatal"},
    };

    // Static configuration setting for the minimum logging level
    LogLevel Logger::minLevel_ = info;

        
    // Ctor:
    // Initializes the logging of a new log line
    Logger::Logger(const LogLevel level)
    : level_(level)
    , os_( level>=LogLevel::warning ? std::cerr : std::clog )
    {
        if (level_ >= minLevel_)
        {    // Start logging only if log level >= minimum level setting
            os_ << logLevelTexts.at(level) << ": ";
        }
    }

    // Dtor:
    // Finalizes the logging of a log line
    Logger::~Logger()
    {
        if (level_ >= minLevel_)
        {    // Finalize logging only if log level >= minimum level setting
            os_ << std::endl;
        }
    }

} // namespace ocs
