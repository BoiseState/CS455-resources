//Author: Amit Jain & Paul Ellis
package main

import (
	"fmt"
	"time"
	"os"
	"net"
)

const MAX_THREADS = 4

type TimeServer struct {
	CONN_HOST string
	CONN_PORT string
	Lnr net.Listener
	ThreadInProgress chan bool
}

//Blocks if there isn't space in the channel for a new thread
func (t *TimeServer)checkNumThreads(){
	t.ThreadInProgress <- true
}

//this will decrease the number of messages in the channel
//call at the end of a go routine
func (t *TimeServer)decNumThreads(){
	<- t.ThreadInProgress
}

func (t *TimeServer) ServiceClients(){
	for {
		//check if we can start a new thread
		t.checkNumThreads()

		//Here we will accept new connections on the socket
		sock, err := t.Lnr.Accept()
		if err != nil {
			fmt.Println("Error accepting:", err.Error())
			os.Exit(1)
		}

		fmt.Println("TimeServer: Received connect from ", sock.RemoteAddr().String())

		//Here we will write the current time to the socket message
		go t.runService(sock)
	}
}

func (t *TimeServer)runService(sock net.Conn){
	fmt.Println("Created New Goroutine")
	sock.Write([]byte(time.Now().String() + "\n"))
	time.Sleep(4 * time.Second)
	sock.Close()
	//Done with work so let another thread run.
	t.decNumThreads()
}

func NewTimeServer(host string, port string) *TimeServer {
	//This starts listening on the port for new TCP connections
	L, err := net.Listen("tcp", ":" + port)
	if err != nil {
		fmt.Println("Error listening:", err.Error())
		os.Exit(1)
	}

	tip := make(chan bool, MAX_THREADS)
	
	ts := TimeServer{CONN_HOST:host, CONN_PORT:port, Lnr:L, ThreadInProgress:tip}
	return &ts
}

func main() {
	timeServer := NewTimeServer("localhost", "5005")
	timeServer.ServiceClients()
}