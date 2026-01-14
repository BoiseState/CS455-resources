//Translation of the Java ThreadExample
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
//We need to pass the WaitGroup by reference (pointer) as they cannot be copied once created
func run(e *Element, wg * sync.WaitGroup) {

	//Here we will launch a new go routine, the Go equivalent of a new thread
	go func(){
		for i := 0; i < 10000; i++ {
			fmt.Println("This is the " + e.name + " thread.")
		}

		//we are finished with the work here so we can tell the work group we are done
		wg.Done()
	}() //don't forget these last two parenthesis as we are calling an anonymous function.


}

func main() {
	//With Go, the entire program will terminate before all proceses will end
	//So we create a group to wait until all processes are done in a go routine
	var wg sync.WaitGroup
	//Add the number of threads to the count of the wait group
	wg.Add(5)

	run(newElement("water"), &wg)
	run(newElement("fire"), &wg)
	run(newElement("earth"), &wg)
	run(newElement("air"), &wg)
	run(newElement("void"), &wg)

	//this will halt the program until we are done.
	wg.Wait()
}
