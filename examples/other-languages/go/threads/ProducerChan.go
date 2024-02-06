//Authors: Amit Jain & Paul Ellis
package main

import (
	"fmt"
	"sync"
	"time"
	"strconv"
)

//A Producer will create messages and insert them into the shared queue
//then it will sleep for a set time and try to insert another message.
type Producer struct{
	id int
	sleepTime int
	Q *SharedQueueTheGoWay
	stopFlag bool
}

func NewProducer(id int, Q *SharedQueueTheGoWay, sleepTime int) *Producer{
	p := Producer{id:id, Q:Q, sleepTime:sleepTime, stopFlag:false}
	return &p
}

func (p Producer)Run(wg *sync.WaitGroup){

	for{
		if p.stopFlag{
			break;
		}
		//We want the time as a string so we format it with UnixDate
		t := time.Now().Format(time.UnixDate)
		p.Q.PutMessage(t)
		msg := "Producer " + strconv.Itoa(p.id) + " put message: " + t
		fmt.Println(msg)
		sT,_ := time.ParseDuration(strconv.Itoa(p.sleepTime) + "ms")
		time.Sleep(sT)
	}

	wg.Done()
}

//this will allow us to prevent the producer from producing any more messages
func (p *Producer)Stop(){
	p.stopFlag = true
}