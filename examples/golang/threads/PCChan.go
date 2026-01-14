//Author: Amit Jain & Paul Ellis
package main

import (
	"fmt"
	"os"
	"sync"
	"strconv"
)

func main(){

	args := os.Args[1:]

	if len(args) != 5 {
		fmt.Println("Usage:$ go build PCChan.go ProducerChan.go ConsumerChan.go SharedQueueTheGoWay.go\n",
		"$ PCChan <queue size> <#producers> <#consumers> <producer_sleep_time(millisecs)> <consumer_sleep_time(millisecs)>")
		os.Exit(1)
	}

	//Convert strings to integers
	maxSize, err := strconv.Atoi(args[0])
	num_producers, err := strconv.Atoi(args[1])
	num_consumers, err := strconv.Atoi(args[2])
	prodSleep, err := strconv.Atoi(args[3])
	conSleep, err := strconv.Atoi(args[4])

	if err != nil{
		fmt.Println("Parsing Error")
		os.Exit(1)
	}

	//we want to store the pointer so that we can pass the
	//same object to both the producer and the consumer
	var Q *SharedQueueTheGoWay

	var wg sync.WaitGroup
	Q = NewSharedQueueTheGoWay(maxSize)

	for i := 0; i < num_producers; i++ {
		wg.Add(1)
		go NewProducer(i, Q, prodSleep).Run(&wg)
	}
	for i := 0; i < num_consumers; i++{
		wg.Add(1)
		go NewConsumer(i, Q, conSleep).Run(&wg)
	}

	wg.Wait()
}