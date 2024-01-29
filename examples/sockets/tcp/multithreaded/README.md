
Go up two levels in the folder hierarchy.

```
cd ../..
```

Running the server:

```
	java tcp.multithreaded.TimeServer  <port>
```

Running the client (on the same or another machine):

```
	java tcp.multithreaded.TimeClient <hostname> <port>
```

Use `localhost` for hostname and `5005` for port.


Use the test.sh script to launch multiple clients in parallel. All other server examples in
this folder can be started in the same way.
