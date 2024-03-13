Multicast Time Server
=====================

The main class is the `time.Controller` class.  There are no command line arguments. Simply
run the following on each participating node:

```
java time.Controller
```

Running this program spawns a client and a server thread.  It also presents the user with a
message to press enter.  Every time the user press enters it causes every client to report
their current time to the server.

Their is one multicast address that is used: 230.230.230.230.  The server listens on 5800 and
the client listens on 5801.

For testing, the application assumes there is only one network interface per server. If not, then
modify the code to provide the interface. 

Use the cscluster machines to test it.

```
ssh cscluster00
cd <to multicasting/ex2-mcast-time folder>
java time.Controller
```

Then repeat for cscluster01 and cscluster02 in two other terminal windows. Then test them together!

It was tested with a maximum of 5 nodes, but we don't see any scaling issues.  It doesn't exit
the multicast group gracefully.



