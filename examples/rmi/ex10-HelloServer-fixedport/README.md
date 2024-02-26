
This shows how to use exportObject with a fixed port to make it easier to allow RMI calls through a
firewall. Using the exportObject method also allows us to avoid having to extend
UnicastRemoteObject.

To compile client and server:

```
make
```

To run:

Start up the registry on the server.

```
export CLASSPATH=$(pwd):$CLASSPATH
rmiregistry <registryPort>
```

The default port is 1099. That is typically used if the registry is being shared by various
servers. Otherwise start up a private copy of the registry using our own assigned port. You
will need to do that on the onyx cluster.


Then start the server with the appropriate security policy:

```
java hello.server.HelloServer <registryPort>&
```

Wait for ready message from the server before running the client. To run the client, use:

```
java hello.client.HelloClient <hostname> <registryPort>
```

The registryPort is only needed if registry is running on a port other than 1099.


