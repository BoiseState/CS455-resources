package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"net"
	"os"
	"time"
)

var (
	DATE_REQUEST_TYPE   = 3
	MY_CALCULATION_TYPE = 4
)

type ServerConnection struct {
	sock *net.Conn
}

func NewServerConnection(s *net.Conn) *ServerConnection {
	sc := ServerConnection{sock: s}
	return &sc
}

func (s *ServerConnection) Run() {
	sock := *s.sock

	fmt.Println("TimeServer: Received connect from ", sock.RemoteAddr().String())
	byteSlice := make([]byte, 1024)

	for {
		n, err := sock.Read(byteSlice)

		if err != nil {
			if err.Error() == "EOF" {
				fmt.Println("Connection closed")
				sock.Close()
				return
			}
			fmt.Println("Error reading:", err)
			return
		}
		test := ProcessRequest(byteSlice[:n])
		fmt.Println("test: ", test)
		fmt.Println("test string: ", string(test))
		sock.Write(test)
	}
}

func ProcessRequest(bs []byte) []byte {
	var buff bytes.Buffer
	var outBuff bytes.Buffer
	dec := json.NewDecoder(&buff)
	enc := json.NewEncoder(&outBuff)

	n := len(bs)

	if n == DATE_REQUEST_TYPE {
		return []byte(time.Now().String())
	} else if n >= MY_CALCULATION_TYPE {
		request := new(MyCalculation)
		buff.Write(bs)
		fmt.Println(buff.Bytes())
		fmt.Println(string(buff.Bytes()))
		err := dec.Decode(request)
		if err != nil {
			fmt.Println("Error decoding:", err.Error())
		}
		fmt.Println(request)
		retVal := request.Execute()
		valStore := NewValueStore()
		valStore.ReturnValue = retVal.([]int)
		fmt.Println(retVal)
		enc.Encode(valStore)
		fmt.Println("encoded val", outBuff.Bytes())
		return outBuff.Bytes()
	} else {
		return nil
	}

}

func main() {
	args := os.Args[1:]
	if len(args) != 1 {
		fmt.Println("Usage: go run Server.go <port>")
		os.Exit(1)
	}

	listener, _ := net.Listen("tcp", ":"+args[0])
	var server *ServerConnection
	for {
		s, _ := listener.Accept()
		server = NewServerConnection(&s)
		go server.Run()
	}
}
