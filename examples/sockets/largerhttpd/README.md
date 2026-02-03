
To start the server:

java LargerHttpd <port> <maxthreads>

Note that the server will only serve files from the current working directory and its
subdirectories. For example, try:

Start the server from the sockets directory with:

java LargerHttpd 5005 4

Then, in another terminal, you can test it with:

java tinyhttpd.Client localhost 5005 largerhttpd/index.html
