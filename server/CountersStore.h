#ifndef OCS_COUNTERS_SERVER_COUNTERS_STORE_H
#define OCS_COUNTERS_SERVER_COUNTERS_STORE_H
//
// CountersStore.h
// ~~~~~~~~~~~~~~~
//
// Header for the CountersStore class:
// - records the number of queries received by the server
// - read/writes this query count to persistent storage
// - can respond to requests for the query current count
//

#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include "Configuration.h"

namespace ocs
{
namespace CountersServer
{

    // CountersStore class:
    // - records the number of queries received by the server
    // - read/writes this query count to persistent storage
    // - can respond to requests for the query current count
    class CountersStore
    {
    public:
        // Ctor:
        // Is meant to be executed at server startup:
        // - opens the persistent storage file
        // - reads the current count stored there by a previous server instance
        // - keeps the file open for later use
        // Caution: may throw if access to persistent storage fails
        CountersStore(const Configuration& configuration);

        // Dtor:
        // Is defaulted: automatically closes the persistent storage file (RAII)
        CountersStore() = default;

        // getCounters():
        // Public API used by the counters server:
        // - receives a client's count request dispatched by a CountersServerDispatcher
        // - increments the query counts,
        // - persists to disk the updated count
        // - returns the updated count to the CountersServerDispatcher
        unsigned long long getCounters();

    private:
        // openPersistentStorage():
        // Function called at startup from the ctor:
        // - opens the persistent storage file
        // - reads the current count stored there by a previous server instance
        // - keeps the file open for later use
        // Caution: may throw if access to persistent storage fails
        void openPersistentStorage();

        // Reference to the structure holding the server startup options
        const Configuration&     configuration_;

        // Internal logic
        std::fstream             persistentStorage_;  // open stream for persistence to disk
        unsigned long long       queries_;            // current query count

        // Since concurrent invocation of getCounters() is possible,
        // the counter and/or the fstream are protected by a common mutex
        mutable std::mutex       countersAndPersistenceMutex_;

        // Filename for persistent storage to disk
        // Note that the filename is fixed:
        // + allows for easy retrieval of a count stored by a previous instance
        // - forbids several servers from running on the same machine (e.g. on different ports)
        static const std::string theFilename_;
    };

} // namespace CountersServer
} // namespace ocs

#endif // OCS_COUNTERS_SERVER_COUNTERS_STORE_H
