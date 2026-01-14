//Author: Amit Jain & Paul Ellis
package main

import (
	"fmt"
	"os"
	"net"
	"bufio"
)

func main(){
	args := os.Args[1:]
	if len(args) != 2 {
		fmt.Println("Usage: go run TimeClient.go <host> <port>")
		os.Exit(1)
	}

	hostPort := args[0] + ":" + args[1]

	//Here we will start a TCP connection to the server
	conn, err := net.Dial("tcp", hostPort)
	if err != nil {
		fmt.Println("Error connecting:", err.Error())
		os.Exit(1)
	}

	//This will read the message from the server
	message, _ := bufio.NewReader(conn).ReadString('\n')
	fmt.Println("Time on host " + args[0] + " is " + message )
	conn.Close()
}