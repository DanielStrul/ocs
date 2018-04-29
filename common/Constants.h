#ifndef OCS_COUNTERS_COMMON_CONSTANTS_H
#define OCS_COUNTERS_COMMON_CONSTANTS_H
//
// Constants.h
// ~~~~~~~~~~~~~~~
//
// Definition of the Constants structure:
// - defines various constants
//

namespace ocs
{

    // Constants structure:
    // Defines various constants
    // No logic is required -> implemented as an open struct
    struct Constants
    {
        // default server and client port
        enum { defaultPort = 12345 };
        
        // default size of reception buffers
        enum { defaultBufferSize = 1024 };
    };

} // namespace ocs

#endif // OCS_COUNTERS_COMMON_CONSTANTS_H
