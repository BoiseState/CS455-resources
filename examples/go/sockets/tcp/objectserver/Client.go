package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"net"
	"os"
	"reflect"
	"strconv"
	"time"
)

func main() {
	//Check the arguments
	args := os.Args[1:]
	if len(args) != 3 {
		fmt.Println("usage: go run Client.go <host> <port> <n>")
		return
	}

	//Setup the variables
	t := time.Now()
	var buff bytes.Buffer
	var outBuff bytes.Buffer
	sliceBytes := make([]byte, 1024)
	enc := json.NewEncoder(&buff)
	dec := json.NewDecoder(&outBuff)
	hostport := args[0] + ":" + args[1]
	n, _ := strconv.Atoi(args[2])
	var decodedVar any

	//Connect to the server
	conn, err := net.Dial("tcp", hostport)
	if err != nil {
		fmt.Println("Error connecting:", err.Error())
		os.Exit(1)
	}

	//Encode a new Date Request to send to the server
	err = enc.Encode(NewDateRequest())
	//Write the encoded Date Request to the server
	num, err := conn.Write(buff.Bytes())
	if err != nil {
		fmt.Println("Error sending:", err.Error())
		os.Exit(1)
	}

	//Read a reply from the server
	num, err = conn.Read(sliceBytes)
	//Write the response from the server to the buffer, limited to the number of bytes read
	outBuff.Write(sliceBytes[:num])
	//Decode the response from the server
	dec.Decode(&decodedVar)
	//Reset the buffers
	buff.Reset()
	outBuff.Reset()
	sliceBytes = make([]byte, 1024)

	/*
	 * Start of the MyCalculation request
	 */

	//Get the current Time
	timeTaken := t.UnixMilli()
	//New MyCalculation request
	myCalc := NewMyCalculation(n)
	//Encode the MyCalculation request
	err = enc.Encode(myCalc)
	//Send the MyCalculation request to the Server
	num, err = conn.Write(buff.Bytes())

	if err != nil {
		fmt.Println("Error sending:", err.Error())
		os.Exit(1)
	}
	//Read the response from the server
	num, err = conn.Read(sliceBytes)
	if err != nil {
		fmt.Println("Error reading:", err.Error())
	}
	//Write the response from the server to the buffer, limited to the number of bytes read
	outBuff.Write(sliceBytes[:num])
	//Get the durration of the request
	timeTaken = time.Now().UnixMilli() - timeTaken
	timeTakenStr := strconv.FormatInt(timeTaken, 10)
	fmt.Println("Time taken to recieve reply from server: " + timeTakenStr + "ms")
	//The Return Value from the server
	retVal := NewValueStore()
	//Decode the response from the server
	dec = json.NewDecoder(&outBuff)
	err = dec.Decode(retVal)
	if err != nil {
		fmt.Println("Error decoding:", err.Error())
	}
	if reflect.TypeOf(retVal.ReturnValue) == reflect.TypeOf([]int{}) {
		if n < 100 {
			fmt.Println(retVal)
			arr := retVal.ReturnValue
			padString := " "
			for i, _ := range arr {
				fmt.Print(strconv.Itoa(arr[i]) + padString)
			}
			fmt.Println()
		}
	} else {
		fmt.Println("Error: Unknown type received from server")
	}

	conn.Close()
}
