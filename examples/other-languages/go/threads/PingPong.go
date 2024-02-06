//Author: Amit Jain & Paul Ellis

package main

import (
	"fmt"
	"time"
	"os"
	"sync"
	"strconv"
)

type PingPong struct {
	word string
	delay int
}

func NewPingPong(word string, delay int) *PingPong{
	p := PingPong{word:word, delay:delay}
	return &p
}

func (p *PingPong)Run(){
	for{
		fmt.Println(p.word + " ")
		d,_ := time.ParseDuration(strconv.Itoa(p.delay)+"ms")
		time.Sleep(d)
	}
}

func main(){
	args := os.Args[1:]
	
	if len(args) != 1{
		fmt.Println("Usage: go run PingPong.go <delay(ms)>")
		os.Exit(1)
	}

	delay,_ := strconv.Atoi(args[0])
	var wg sync.WaitGroup

	wg.Add(1)
	go NewPingPong("ping", delay).Run()
	go NewPingPong("PONG", delay).Run()

	wg.Wait()
}