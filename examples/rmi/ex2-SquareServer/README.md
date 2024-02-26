

To compile:

```
make
```

To run:

Start up the registry on the server

```
export CLASSPATH=$(pwd):$CLASSPATH
rmiregistry <registryPort>
```

The default port is 1099. That is typically used if the registry is being shared by various servers on the same machine. Otherwise start up a private copy of the registry using your own port in the range assigned to your team.


Then start the server:

```
java square.server.SquareServer <registryPort> &
```

Wait for ready message from the server before running the client. Then run the client as follows:

```
java square.client.SquareClient <server host> <value> <count> <registryPort>
```



