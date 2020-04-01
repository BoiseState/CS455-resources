
To build and run
----------------

To build: 

make

To run:

Use the script run-server to run the server.

Use the script run-client to run the client.

Use the script test-multiple-clients.sh to test multi-threadedness of RMI implementation

Note that these scripts assume that the rmiregistry runs on the default 1099 port. If you are using
a custom port, then modify the server and client scripts.


Dynamic classloading
--------------------

For classes to automagically upload/download, we need to have the server run on a webserver
(and the codebase should be accessibly by the webserver). Similarly, the client would have to
run on a webserver as have its codebase be accessible by that web server as well.

In this case, use the following scripts:

run-server-dynamic.sh
run-client-dynamic.sh


Note: If you do start the rmiregistry and it can find your stub classes in CLASSPATH, it will not
remember that the loaded stub class can be loaded from your server's code base, specified by
the java.rmi.server.codebase property when you started up your server application. Therefore,
rmiregistry will not convey to clients the true code base associated with the stub class
and, consequently, your clients will not be able to locate and to load the stub class or other
server-side classes.



