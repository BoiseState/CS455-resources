

To build: 

make

To run:

Use the script run-server.sh to run the server.

run-server.sh

Use the script run-client.sh to run the client. 

run-client.sh localhost

Run several copies together using the test-multiple-clients.sh script to
see the affect of asynchronous thread exceution.

test-multiple-clients.sh

NOTE: This example assumes that the registry is running on port 1099. If that is not the case,
change the following code similar to the change in ../ex3-Client-Callback
