//
// CountersStore.cpp
// ~~~~~~~~~~~~~~~
//
// Source for the CountersStore class:
// - records the number of queries received by the server
// - read/writes this query count to persistent storage
// - can respond to requests for the query current count
//
#include "CountersStore.h"
#include <cstring>
#include <ctime>
#include <iostream>
#include <memory>
#include "Logger.h"


namespace ocs
{
namespace CountersServer
{

    // Filename for persistent storage to disk
    // Note that the filename is fixed:
    // + allows for easy retrieval of a count stored by a previous instance
    // - forbids several servers from running on the same machine (e.g. on different ports)
    const std::string CountersStore::theFilename_ = "query_counters.txt";


    // Ctor:
    // Is meant to be executed at server startup:
    // - opens the persistent storage file
    // - reads the current count stored there by a previous server instance
    // - keeps the file open for later use
    // Caution: may throw if access to persistent storage fails
    CountersStore::CountersStore(const Configuration& configuration)
    : configuration_(configuration)
    , persistentStorage_()
    , queries_(0)
    , countersAndPersistenceMutex_()
    {
        openPersistentStorage();
    }


    // getCounters():
    // Public API used by the counters server:
    // - receives a client's count request dispatched by a CountersServerDispatcher
    // - increments the query counts,
    // - persists to disk the updated count
    // - returns the updated count to the CountersServerDispatcher
    unsigned long long CountersStore::getCounters()
    {
        unsigned long long result = 0;

        // Since concurrent invocation of getCounters is possible,
        // the counter's and fstream's common mutex need be locked
        {
            std::lock_guard<std::mutex> lock(countersAndPersistenceMutex_);
            result = ++queries_;
            persistentStorage_.seekp(0);
            persistentStorage_ << queries_;
            persistentStorage_.flush();
        }

        return result;
    }

    // openPersistentStorage():
    // Function called at startup from the ctor:
    // - opens the persistent storage file
    // - reads the current count stored there by a previous server instance
    // - keeps the file open for later use
    // Caution: may throw if access to persistent storage fails
    void CountersStore::openPersistentStorage()
    {
        // Compute the file path
        auto directory = configuration_.workDirectory;
        if (!directory.empty() && directory.back() != '/')
            directory += '/';
        const auto filepath = directory + theFilename_;

        // Try and open the stream for read/write
        persistentStorage_.open(filepath);

        // If the stream is not open, try and create it
        if(!persistentStorage_.is_open())
        {
            // Try and force the creation of a new, empty, file
            persistentStorage_.clear();
            persistentStorage_.open(filepath, std::ios::out);

            // A new storage file must be created with a valid (non-empty) content
            persistentStorage_ << 0 << std::endl;

            // Close and reopen in rea/write mode
            persistentStorage_.close();
            persistentStorage_.open(filepath);
        }

        // If the stream is not open, abort on error
        if(persistentStorage_.fail())
        {
            const auto msg = std::string("Could not open the persistent storage file: ") + std::strerror(errno);
            Logger(error) << msg;
            throw std::logic_error(msg);
        }

        // Try and read the current query count
        persistentStorage_ >> queries_;
        if (persistentStorage_.fail())
        {
            const auto msg = "Could not read the current query count from the persistent storage file";
            Logger(error) << msg;
            throw std::logic_error(msg);
        }

        Logger(info) << "Query count was read from the persistent storage file: " << queries_;
    }

} // namespace CountersServer
} // namespace ocs
