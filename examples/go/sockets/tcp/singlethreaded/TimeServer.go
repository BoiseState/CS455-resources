//Author: Amit Jain & Paul Ellis
package main

import (
	"fmt"
	"time"
	"os"
	"net"
)


type TimeServer struct {
	CONN_HOST string
	CONN_PORT string
	Lnr net.Listener

}

func (t TimeServer) ServiceClients(){
	for {
		//Here we will accept new connections on the socket
		sock, err := t.Lnr.Accept()
		if err != nil {
			fmt.Println("Error accepting:", err.Error())
			os.Exit(1)
		}

		fmt.Println("TimeServer: Received connect from ", sock.RemoteAddr().String())

		//Here we will write the current time to the socket message
		sock.Write([]byte(time.Now().String() + "\n"))
		time.Sleep(4 * time.Second)
		sock.Close()
	}
}

func NewTimeServer(host string, port string) *TimeServer {
	//This starts listening on the port for new TCP connections
	L, err := net.Listen("tcp", ":" + port)
	if err != nil {
		fmt.Println("Error listening:", err.Error())
		os.Exit(1)
	}

	
	ts := TimeServer{CONN_HOST:host, CONN_PORT:port, Lnr:L}
	return &ts
}

func main() {
	timeServer := NewTimeServer("localhost", "5005")
	timeServer.ServiceClients()
}