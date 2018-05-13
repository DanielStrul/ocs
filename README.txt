OCS
===

Content: The project OCS is a client/server exercice
Author:  Daniel Strul
Version: Demo 0.2, 2018/05/13

Content
-------
There are 3 subpackages in 3 separate subdirectories:
    server: a small UDP/V6 server, that:
            - listens on a UDP port;
            - can process 'GET' queries;
            - keeps a query counter in memory and persisted on disk;
            - increments the counter each time it receives a 'GET' query;
            - then sends back the updated counter to the client.
    client: a small UDP/V6 synchronous client that can poll a server (as
            described above) every 5 seconds with a 'GET' query
    common: a small library of components and configuration settings shared
            between the client and the server

There is an additional subdirectory:
    doc:    Miscellaneous docs (currently, only some results of profiling tests)


Requirements
------------
This package uses gcc and BOOST, assumed to be installed in the path and was
tested in an AWS EC2 environment:
    g++ 7.3.1
    boost 1.53.0-27


Building
--------
There is no 'configure' file yet, so building is done by launching 'make', and 
tweaking the Makefile options if ever there is an issue.
If the build succeeds, it builds two version of both the client and the server 
programs, in two separate subdirectories:

    root-directory
        |
        |- build
            |
            |- debug
            |    |
            |    |- bin
            |        |
            |        |- client
            |        |- server
            |
            |- release
                |
                |- bin
                    |
                    |- client
                    |- server

You may also build a version dedicated to grprof profiling by launching 'make gprof', which
builds both the client and the server programs, in a separate subdirectory:

    root-directory
        |
        |- build
            |
            |- gprof
                |
                |- bin
                    |
                    |- client
                    |- server


Executing the programs
----------------------
The client and the server are launched independently, with some launch options available:

    ./build/release/bin/server --help
    Usage: server [options]
    Allowed options:
      --port arg            set the udp port on which to listen (default: 12345)
      --work-directory arg  set the work-directory for the persistent storage file
                            (default: current directory)
      --log-level arg       set the log-level from -2 for trace to 3 for fatal
                            (default: 0 for info)

    ./build/release/bin/client --help
    Usage: client [options]
    Allowed options:
      --help                produce this help message
      --host arg            set the name/ip of the target server (default:
                            localhost)
      --service arg         set the udp port or service name on the target server
                            (default: 12345)
      --log-level arg       set the log-level from -2 for trace to 3 for fatal
                            (default: 0 for info)


Testing both programs
---------------------
No unit tests were included yet.
So testing relies on:
1) launching the server in a shell, which listens on port 12345 by default 
2) launching the client in a 2nd shell, which polls the server on port 12345 by default 
3) checking that they seem to be talking to each other every 5 seconds, as expected...

Slightly more execution details are made available when launching the client and/or the server at debug log level (-1)

    ./build/release/bin/server --log-level -1
    ./build/release/bin/client --log-level -1


Clientless server test
----------------------
The server may actually be tested without the client using nc:

Shell 1> ./build/release/bin/server
Shell 2> nc -u ::1 12345 <<< "GET"


Profiling examples
------------------
There are various examples of profiling scripts in 'doc/Performance_profiling.xlsx'.
For example, for a vary simple test with 'time':
    echo "0" > query_counters.txt; \
    (time build/release/bin/server &); \
    (for i in {1..1000}; do nc -u ::1 12345  > /dev/null <<< "GET"; done); \
    pkill -INT server
