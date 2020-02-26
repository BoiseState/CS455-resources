
If you start the server and see the following error:

$ ./server 
Cannot register service: RPC: Authentication error; why = Client credential too weak
unable to register (SQUARE_PROG, SQUARE_VERS, udp).
$

then add the following line to /etc/sysconfig/rpcbind

RPCBIND_ARGS=-i

and then restart the rpcbind service

/sbin/systemctl restart rpcbind 


To run the client on the localhost:

client localhost <integer value>

To run a client that makes n calls (for timing purposes)

client_test localhost <integer value> <n>

The client_test.c program makes n calls to the RPC for timing purposes.

