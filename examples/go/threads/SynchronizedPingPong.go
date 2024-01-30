//Author: Amit Jain & Paul Ellis

package main

import (
	"fmt"
	"sync"
	"time"
	"strconv"
	"os"
)

type Ping struct{
	word string
	delay time.Duration
	ping *chan bool
	pong *chan bool
}

type Pong struct{
	word string
	delay time.Duration
	ping *chan bool
	pong *chan bool
}

func NewPing(word string, delay int, ping *chan bool, pong *chan bool) *Ping{
	//Here we need to convert an integer to a time.Delay type for time.Sleep
	t,_ := time.ParseDuration(strconv.Itoa(delay) + "ms")
	p := Ping{word:word, delay:t, ping:ping, pong:pong}
	return &p
}

func NewPong(word string, delay int, ping *chan bool, pong *chan bool) *Pong{
	//Here we need to convert an integer to a time.Delay type for time.Sleep
	t,_ := time.ParseDuration(strconv.Itoa(delay) + "ms")
	p := Pong{word:word, delay:t, ping:ping, pong:pong}
	return &p
}

func (p *Ping) Run(){
	for {
		//wait for pong to send something to the Pong channel
		//The channel will block until it gets a value
		<- *p.pong
		fmt.Println(p.word + " ")
		time.Sleep(p.delay)
		//let pong know ping is done
		*p.ping <- true
	}
}

func (p *Pong)Run(){
	for {
		//wait for ping to send something to the ping channel
		//The channel will block until it gets a value
		<- *p.ping
		fmt.Println(p.word + " ")
		time.Sleep(p.delay)
		//let ping know pong is done
		*p.pong <- true
	}
}

func main(){
	args := os.Args[1:]

	if len(args) != 1 {
		fmt.Println("Usage: $ go run SynchronizedPingPong.go <delay(ms)>")
		os.Exit(1)
	}

	//Get the delay as an integer, the underscore means that we don't
	//care if strconv.Atoi returns an error, just ignore that return val
	delay, _ := strconv.Atoi(args[0])

	//Here we are making channels that different go routines can talk
	//to each other with. A channel will block until it can recieve or
	//Send which makes it useful for this situation.
	pingChan := make(chan bool)
	pongChan := make(chan bool)

	var wg sync.WaitGroup

	ping := NewPing("ping", delay, &pingChan, &pongChan)
	pong := NewPong("pong", delay, &pingChan, &pongChan)

	//You have to give one channel a "kickstart" from another go routine
	go func(){pingChan <- true}()

	//Run infinitely as we never call wg.Done()
	wg.Add(1)
	go ping.Run()
	go pong.Run()
	wg.Wait()
}