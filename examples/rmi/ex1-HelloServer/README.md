

To compile client and server:

```
make
```

To run:


The default port or rmiregistry is 1099. That is typically used if the registry is being shared by various
servers. Otherwise we can start up a private copy of the registry using our own assigned port. We do
that in the main method in the server code.


To run the server, use (e.g. use 5005 as the registry port):

```
java hello.server.HelloServer <registryPort>
```

Wait for ready message from the server before running the client. To run the client, in a seprate
terminal, use:

```
java hello.client.HelloClient <hostname> <registryPort>
```

The registryPort is only needed if the rmiregistry is running on a port other than 1099.


