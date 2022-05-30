
Go up two levels in the folder hierachy.

cd ../..

Running the server (uses hard-coded port 5005):

	java tcp.singlethreaded.TimeServer 

Running the client (on same or another machine):

	java tcp.singlethreaded.TimeClient <hostname> <port>

Use localhost for hostname and 5005 for port.
