

To compile client and server:

```
make
```

To run:

Start up the registry on the server.

```
export CLASSPATH=$(pwd):$CLASSPATH
rmiregistry <port>
```

The default port is 1099. That is used if the registry is being shared by various
servers. Otherwise start up a private copy of the registry using our own assigned port and
modify the server and client code appropriately.


Then start the server with the appropriate security policy:

```
java hello.server.Server &
```

Wait for ready message from the server before running the clients. This server will serve clients with two interfaces, the english and french!  To run the clients, use:

```
java hello.client.HelloClient hostname [registryPort]
java hello.client.BonjourClient hostname [registryPort]
```

The registryPort is only needed if registry is running on a port other than 1099.

If you compiled the server in a package, then provide appropriate classpath in the RMI codebase
VM argument. This assumes that server class files are in a folder that is accessible via a web
server (running on the same system where the RMI server will run)

```
java -classpath classDir -Djava.rmi.server.codebase=file:classDir/  Server & 
```

