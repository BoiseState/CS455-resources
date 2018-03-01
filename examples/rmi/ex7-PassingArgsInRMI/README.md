To compile client and server:

make

To run:

Start up the registry on the server.

export CLASSPATH=$(pwd):$CLASSPATH
rmiregistry <port>

The default port is 1099. That is used if the registry is being shared by various
servers. Otherwise start up a private copy of the registry using our own assigned port.


Then start the server with the appropriate security policy:

java -Djava.security.policy=security.policy Server &

Wait for ready message from the server before running the client. To run the
client, use:

java -Djava.security.policy=security.policy Client hostname [registryPort]

The registryPort is only needed if registry is running on a port other than 1099.


