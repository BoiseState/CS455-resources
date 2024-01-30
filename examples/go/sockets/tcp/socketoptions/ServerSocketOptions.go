package main

import (
	"context"
	"fmt"
	"net"
	"os"
	"syscall"
	"time"

	"golang.org/x/sys/unix"
)

func main() {

	//This allows us to have multiple connections on the smae IP address
	lc := net.ListenConfig{
		//Here we are telling the ListenerConfig that the contol field will be an anonymous function
		Control: func(network, address string, c syscall.RawConn) error {
			return c.Control(func(fd uintptr) {
				//Here we have to call an external library to set the socket option through the kernel
				//This will allow us to reuse addresses for connections
				err := unix.SetsockoptInt(int(fd), unix.SOL_SOCKET, unix.SO_REUSEADDR, 1)
				if err != nil {
					fmt.Println("Error setting socket option:", err.Error())
				}
			})
		}}

	//Now we create the listener using the ListenConfig. context.Background is a blank context that
	//the ListenConfig can populate with the proper context items. You should never pass a nil context
	listener, err := lc.Listen(context.Background(), "tcp", ":5005")
	if err != nil {
		fmt.Println("Error listening:", err.Error())
		os.Exit(1)
	}

	//Here we set the deadline for the listener. This is a timeout for the listener to wait for a connection
	//After 1 minute the listener will timeout and return an error
	listener.(*net.TCPListener).SetDeadline(time.Now().Add(1 * time.Minute))

	serverClients(&listener)
}

func serverClients(listener *net.Listener) {
	shutdown := false
	l := *listener
	for {
		if shutdown {
			break
		}
		conn, err := l.Accept()
		if err != nil {
			//here we will check if the error is an OpError and if it is specifically a timeout error
			if terr, ok := err.(*net.OpError); ok && terr.Timeout() {
				fmt.Println("accept timeout")
				shutdown = true
				os.Exit(1)
			} else {
				fmt.Println(err.Error())
				os.Exit(1)
			}
		}
		fmt.Println("Connect from " + conn.RemoteAddr().String())
	}
}
