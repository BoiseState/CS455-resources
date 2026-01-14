//Author: Amit Jain & Paul Ellis
package main

import (
	"sync"
)

type SharedQueue struct{
	queue []string
	maxSize int
	mu sync.Mutex
}

func NewSharedQueue(maxSize int) *SharedQueue{
	//When making the new slice for strings we provide the size
	q := SharedQueue{queue:make([]string, maxSize), maxSize:maxSize}
	return &q
}

func (q *SharedQueue)Size() int{
	var s int
	q.mu.Lock()
	s = len(q.queue)
	q.mu.Unlock()
	return s
}

func (q *SharedQueue)PutMessage(msg string){
	//Here we are waiting for the queue to have space for a new message
	for{
		if q.Size() != q.maxSize {
			break
		}
	}
	q.mu.Lock()
	q.queue = append(q.queue, msg)
	q.mu.Unlock()
}

func (q *SharedQueue)GetMessage() string{
	//We will wait for there to actually be a message in the queue
	for{
		if q.Size() != 0{
			break
		}
	}
	var msg string
	q.mu.Lock()
	msg = q.queue[0]
	//this sets the queue to be the queue from index 1 to the end
	//effectively removing the head from the queue
	q.queue = q.queue[1:]
	q.mu.Unlock()
	return msg
}