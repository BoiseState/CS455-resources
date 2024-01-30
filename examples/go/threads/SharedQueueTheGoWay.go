//Author: Amit Jain & Paul Ellis
package main

import (
	"sync"
)

type SharedQueueTheGoWay struct{
	queue chan string
	maxSize int
	currentSize int
	mu sync.Mutex
}

func NewSharedQueueTheGoWay(maxSize int) *SharedQueueTheGoWay{
	//When making the new Channel for strings we provide the size for the buffer
	q := SharedQueueTheGoWay{queue:make(chan string, maxSize), maxSize:maxSize, currentSize:0}
	return &q
}

func (q *SharedQueueTheGoWay)Size() int{
	var s int
	q.mu.Lock()
	s = q.currentSize
	q.mu.Unlock()
	return s
}

func (q *SharedQueueTheGoWay)PutMessage(msg string){
	//Channel is blocking so we don't need to lock
	q.queue <- msg
	//We want to lock here because the size is not atomic
	q.mu.Lock()
	q.currentSize++
	q.mu.Unlock()
}

func (q *SharedQueueTheGoWay)GetMessage() string{
	//Channel is blocking so we don't need to lock
	msg := <- q.queue
	//We want to lock here because the size is not atomic
	q.mu.Lock()
	q.currentSize--
	q.mu.Unlock()
	return msg
}