#ifndef OCS_COMMON_LOGGER_H
#define OCS_COMMON_LOGGER_H

//
// Logger.h
// ~~~~~~~~~~
//
// Header for the Logger class, which serves two purposes:
// In general: is meant to provide a thin wrapper around some concrete logger facility or library
// Meant to allow for changing the logger (for e.g. Boost or Google) without too much 
// hassle in the client code
// In my case: is meant to provide me with a logger despite the fact that the boost logger 
// is not willing to compile on my box ;-)
//

#include <memory>
#include <ostream>
#include <unordered_map>

namespace ocs
{
    // LogLevel enumeration:
    // Definition of the logging levels (see Logger.h for details)
    enum LogLevel
    {
        trace = -2,
        debug,
        info,
        warning,
        error,
        fatal
    };

    // Logger: 
    // In general: is meant to provide a thin wrapper around some concrete logger facility or library
    // Meant to allow for changing the logger (for e.g. Boost or Google) without too much 
    // hassle in the client code
    // In my case: is meant to provide me with a logger despite the fact that the boost logger 
    // is not willing to compile on my box ;-)
    class Logger
    {
    public:
        // Ctor:
        // Initializes the logging of a new log line
        Logger(const LogLevel level);

        // Dtor:
        // Finalizes the logging of a log line
        ~Logger();

        // insertion operator template:
        // Note: since a Logger is always meant to be used as a temporary
        // object, only the rvalue-reference case is handled.
        template<class T>
        friend Logger&& operator<<(Logger&& handler, const T& msg)
        {
            if (handler.level_ >= Logger::minLevel_)
            {    // Log msg only if log level >= minimum level setting
                handler.os_ << msg;
            }
            return std::move(handler);
        }

        // Setter for the minimum log level
        static void setMinLevel(const LogLevel value)
        {
            minLevel_ = value;
        }

    private:
        LogLevel        level_; // Logging level for the current line
        std::ostream&   os_;    // stream for the current line (cout, cerr, ...)

        // Static map of log-level names
        static const std::unordered_map<LogLevel,const std::string> logLevelTexts;

        // Static configuration setting for the minimum logging level
        static LogLevel minLevel_;
    };

} // namespace ocs

#endif // OCS_COMMON_LOGGER_H
