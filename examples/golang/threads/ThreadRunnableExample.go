//Translation of the Java ThreadRunnableExample
//Author: Amit Jain & Paul Ellis
package main

import (
	"fmt"
	"sync"
)

//An Element stores a name
type Element struct {
	name string
}
//Create a new Element
func newElement(name string) *Element {
	e := Element{name: name}
	return &e
}
//Run and print out the Thread
func run(e *Element, wg * sync.WaitGroup) {

	for i := 0; i < 10000; i++ {
		fmt.Println("This is the " + e.name + " thread.")
	}
	wg.Done()
}

func main() {

	var wg sync.WaitGroup

	wg.Add(5)

	//Start the run function in a new "thread" called a goroutine
	go run(newElement("water"), &wg)
	go run(newElement("fire"), &wg)
	go run(newElement("earth"), &wg)
	go run(newElement("air"), &wg)
	go run(newElement("void"), &wg)

	wg.Wait()
}