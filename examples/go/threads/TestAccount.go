//Author: Amit Jain & Paul Ellis

package main

import(
	"fmt"
	"os"
	"sync"
	"strconv"
)

func main(){
	//get Command line arguments, 0 is the program name
	//doing it this way get Args 1 to the end
	args := os.Args[1:]

	if len(args) != 3{
		fmt.Println("Usage: go run TestAccount.go <numRoutines> <iterations> <good|bad>")
		os.Exit(1)
	}

	var numRoutines int
	var iterations int

	//Here we will convert the String to Int and catch if the String is not an int
	numRoutines, err:= strconv.Atoi(args[0])
	if err != nil {
		fmt.Println("Number of Routines should be an Integer!")
		os.Exit(1)
	}

	iterations, err = strconv.Atoi(args[1])
	if err != nil {
		fmt.Println("Error: Number of iterations should be an Integer!")
		os.Exit(1)
	}

	if numRoutines > 32 {
		fmt.Println("Maximum number of threads allowed is 32!")
		os.Exit(1)
	}

	var account Account

	if args[2] == "good" {
		account = newSynchronizedAccount()
	} else if args[2] == "bad" {
		account = newBasicAccount()
	} else {
		fmt.Println("Usage: go run TestAccount.go <numRoutines> <iterations> <good|bad>")
	}

	var wg sync.WaitGroup
	
	for i := 0; i < numRoutines; i++ {
		wg.Add(1)
		go TestThread(&account, iterations,	&wg)
	}

	wg.Wait()

	fmt.Println(account.ToString())
	
}

func TestThread(account *Account, iterations int, wg *sync.WaitGroup){
	//Go can infer types like floats values with the := opperator
	amount := 1.0
	for t := 0; t < iterations; t++{
		(*account).Deposit(amount)
	}
	wg.Done()
}