--------
This is a modification of ex1 and demonstrates how to setup a timeout for a RMI call.


To compile client and server:

```
make
```

To run:

Stop already running rmiregistry:

```
killall -9 rmiregistry
```

Start up new registry on the server:

```
export CLASSPATH=$(pwd):$CLASSPATH
rmiregistry <port>
```

The default port is 1099. That is used if the registry is being shared by various
servers. Otherwise start up a private copy of the registry using your own assigned port and
modify the server and client appropriately.


Then start the server:

```
java hello.server.HelloServer &
```

Wait for ready message from the server before running the client. To run the
client, use:

```
java hello.client.HelloClient hostname [registryPort]
```

The registryPort is only needed if registry is running on a port other than 1099.

