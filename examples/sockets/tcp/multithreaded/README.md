
Go up two levels in the folder hierachy.

cd ../..

Running the server:

	java tcp.multithreaded.TimeServer  <port>

Running the client (on same or another machine):

	java tcp.multithreaded.TimeClient <hostname> <port>

Use localhost for hostname and 5005 for port.


Use the test.sh script to launch multiple clients in parallel. All tother servr examples in thie folder can be started the same way.
