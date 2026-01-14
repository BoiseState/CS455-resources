//Author: Amit Jain & Paul Ellis
package main

import (
	"fmt"
	"sync"
	"time"
	"strconv"
)

//A consumer will remove messages from the queue and sleep
//before trying again.
type Consumer struct{
	id int
	Q *SharedQueueTheGoWay
	sleepTime int
	stopFlag bool
}

func NewConsumer(id int, Q *SharedQueueTheGoWay, sleepTime int) *Consumer{
	c := Consumer{id:id, Q:Q, sleepTime:sleepTime, stopFlag:false}
	return &c
}

func (c *Consumer)Run(wg *sync.WaitGroup){
	//this is an infinite loop in go, there are no while loops in go
	for{
		if c.stopFlag {
			break;
		}

		msg := c.Q.GetMessage()
		fmt.Println("Consumer " + strconv.Itoa(c.id) + " got message: " + msg)
		sT,_ := time.ParseDuration(strconv.Itoa(c.sleepTime)+"ms")
		time.Sleep(sT)
	}
	wg.Done()
}

func (c *Consumer)Stop(){
	c.stopFlag = true
}
