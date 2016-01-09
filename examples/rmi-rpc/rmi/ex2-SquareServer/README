

To compile:


ant build.xml

To run:

Start up the registry on the server

export CLASSPATH=`pwd`:$CLASSPATH
rmiregistry <port>

The default port is 1099. That is used if the registry is being shared by
various servers. Otherwise start up a private copy of the registry using
our own assigned port.


Then start the server with the appropriate security policy:

java -Djava.security.policy=mysecurity.policy square.server.SquareServer &

Wait for ready message from the server before running the client.



Make sure you build the server first as the client needs SquareServer_Stub
class from the server side.

The rmiregistry and server should be already running. Then use

java square.client.SquareClient hostname value count [registryPort]

The registryPort is only needed if registry is running on a port other than 1099.


