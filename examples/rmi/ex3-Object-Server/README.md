
RMI example3: Overview
----------------------

This example demonstrates a a client that makes synchronous RMI calls to a RMI server. The RMI server is multithreaded (like any RMI server). The assumption is that the relevant class files are available on both server and client.

To build and run
----------------

To build: 

make

To run:

Use the script run-server to run the server. If no registry port parameter is provided, then we assume that the default port of 1099 is being used.

./run-server.sh [<registry port>]

Use the script run-client to run the client. If no registry port parameter is provided, then we assume that the default port of 1099 is being used.

./run-client.sh <server host> [<registry port>]

Use the script ./test-multiple-clients.sh to observe multithreaded behavior of RMI implementation

./test-multiple-clients.sh  <server host> [<registry port>]
