OCS
===

Content: The project OCS is a client/server exercice
Author:	 Daniel Strul
Version: Demo, 2018/04/27

Content
-------
There are 3 subpackages in 3 separate subdirectories:
	server:	a small UDP/V6 server, that:
			- listens on a UDP port;
			- can process 'GET' queries;
			- keeps a query counter in memory and persisted on disk;
			- increments the counter each time it receives a 'GET' query;
			- then sends back the updated counter to the client.
	client:	a small UDP/V6 synchronous client that can poll a server (as
		    described above) every 5 seconds with a 'GET' query
	common: a small library of components and configuration settings shared
			between the client and the server

			
Requirements
------------
This package uses gcc and BOOST (assumed to be installed in the path).
It was unfortunately developped on CygWin, so it is for now only validated
for the limited toolchain that was available there:
	g++ 6.4.0
	boost 1.67.0
	
	
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
			|	|
			|	|- bin
			|		|
			|		|- client
			|		|- server
			|
			|- release
				|
				|- bin
					|
					|- client
					|- server


Executing the programs
----------------------
The client and the server are launched independently, with a few options:

	./build/release/bin/server --help
	Usage: server [options]
	Allowed options:
	  --help                produce this help message
	  --port arg            set the udp port on which to listen (12345 by default)
	  --work-directory arg  set the work-directory for the persistent storage file
							(/var/run/)
	  --log-level arg       set the log-level from -2 for trace to 3 for fatal
							(default, 0 for info)

	./build/release/bin/client --help
	Usage: client [options]
	Allowed options:
	  --help                produce this help message
	  --host arg            set the name/ip of the target server (localhost by
							default)
	  --service arg         set the udp port or service name on the target server
							(12345 by default)
	  --log-level arg       set the log-level from -2 for trace to 3 for fatal
							(default, 0 for info)

							
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

