//Author: Amit Jain & Paul Ellis
package main

import (
	"fmt"
	"os/exec"
	"strconv"
)

const MAX_PROCESSES = 5

func main(){

	//Create a slice to hold 5 processes
	procs := make([]*exec.Cmd, MAX_PROCESSES)
	
	//create and start a sleep command for each space in the slice
	//and save to the slice
	for i := 0; i < MAX_PROCESSES; i++{
		procs[i] = exec.Command("sleep", "30")
		procs[i].Start()
		fmt.Println("Child Process " + strconv.Itoa(i) + " created")
	}
	fmt.Println()

	//wait for each command in the slice to finish
	for i := 0; i < MAX_PROCESSES; i++{
		procs[i].Wait()
		fmt.Println("Child Process " + strconv.Itoa(i) + " finished")
	}

	fmt.Println()
}

