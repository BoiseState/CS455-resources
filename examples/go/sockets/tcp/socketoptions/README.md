# TCP Socket Options
## Differences to Java
The socket options in go are a lot more limited than in Java.
The only socket options that are fairly trivial to set are the timeout/Deadline options.
The difference between timeout and deadline is that timeout is the time one specific connection will wait
while the deadline is the time all connections will fail. Another big difference is that there is no
way I could find to read the specifics of a connection outside of the address being connected to.
With a dialer you can read certain properties like Timeout and Deadline but not information such as the
packet size.

## Recomendations
If you need to use specific options for the socket use Java not Go as the control is easier with Java.
In order to control more of the Go socket options look into the Context and Control fields in the Go
documentation as those will allow you to call kernel level commands to change the options for sockets.

## Running this example
In order to run these examples you will have to run the command:
`go mod download`
This will download the package used to call unix system commands from inside the program


You can run the server in this example with the command
`go run ServerSocketOptions.go`

The Client can be run using the command:
`go run ClientSocketOptions.go [<server host>]`

This example uses port 5005 and won't run if the port is currently in use by other programs.