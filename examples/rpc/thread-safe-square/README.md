
This example demonstrates that the default implementation of the RPC server is
single-threaded.

Run the server:

./server

Run multiple clients together

./client localhost 2 &
./client localhost 2 &
./client localhost 2 &
./client localhost 2 &

Then watch the output from the server.


