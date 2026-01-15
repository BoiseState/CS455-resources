//Author: Amit Jain & Paul Ellis
package main

import (
	"fmt"
	"sync"
	"time"
)

const MAX = 1000000

func run(i int, wg *sync.WaitGroup){
	fmt.Println("Thread Number: ", i)
	time.Sleep(time.Second*20)
	wg.Done()
}

func main(){

	var wg sync.WaitGroup

	//Start as many go routines as we can
	for i := 0; i< MAX; i++{
		wg.Add(1)
		go run(i, &wg)
	}

	wg.Wait()

}
