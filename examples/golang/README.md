# CS455-555-Go-Examples
Examples for the CS 455-555 Distributed Systems class transposed from various languages to Go

## Information
In order to run most examples you can use the command `go run ExampleFile.go`. In some cases when there are multiple files included in an example you will want to build the example then run the build. In order to do this use the command `go build MainFile.go Dependency1.go ...`, then you can run the command `./MainFile arg1 arg2 ...`.

For these examples you can see in the go.mod file that they were developed using Go version 1.21.4 This is the version of go that you should be using. However if the version isn't installed on the machine you can change the mod file to use the same version of Go as is installed on the machine. Warning this may break some examples however.

If an example has dependencies that aren't downloaded yet you can use the command `go mod downlad` to download the dependencies needed to run the example.

Some examples will have a README file in the directory with them. That will help explain some of the concepts or differences with that example and some troubleshooting steps if there are issues.

For all socket examples you may need to change the sockets used in order for the example to work due to confilcts with the socket being used on another process already.
