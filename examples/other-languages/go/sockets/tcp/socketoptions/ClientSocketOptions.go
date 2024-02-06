package main

import (
	"context"
	"fmt"
	"net"
	"os"
	"time"
)

func main() {
	serverHost := "localhost"

	args := os.Args[1:]
	if len(args) == 1 {
		serverHost = args[0]
	} else {
		fmt.Println("Usage: go run ClientSocketOptions [<server host>]")
		os.Exit(1)
	}

	serverHost = serverHost + ":5005"

	// We can create a dialer with specific fields such as timeout to set the timeout for the connection
	dialer := net.Dialer{Timeout: time.Second * 5}

	//Another way is to use a context with timeout. This is the preferred method
	//Here we will create a context with a timeout of 5 seconds
	context, cancel := context.WithTimeout(context.Background(), time.Second*5)
	defer cancel()

	//Here we will use the DialContext function to connect to the server using the context with a timeout
	conn, err := dialer.DialContext(context, "tcp", serverHost)
	if err != nil {
		fmt.Println("Error connecting:", err.Error())
		os.Exit(1)
	}
	fmt.Println("Dialer timeout: ", dialer.Timeout)
	//Unfortunately there is no way to read the socket options from an existing connection so we will just print
	//out the connection information
	fmt.Println("Socket conn")
	fmt.Println("Connection Timeout is 5 seconds")
	fmt.Println("Using local address:port " + conn.LocalAddr().String())
	conn.Close()
}
