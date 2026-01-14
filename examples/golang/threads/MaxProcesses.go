//Author: Amit Jain & Paul Ellis

package main

import (
	"fmt"
	"os/exec"
	"strconv"
)

const MAX_PROCESSES = 50000

func main(){

	//Creating a slice to hold exec.Cmd pointer types
	procs := make([]*exec.Cmd, MAX_PROCESSES)
	
	for i := 0; i < MAX_PROCESSES; i++{
		//set the item and the command for it
		procs[i] = exec.Command("sleep", "30")
		//start the command in the slice in a new process
		procs[i].Start()
		fmt.Println("Child Process " + strconv.Itoa(i) + " created")
	}
	fmt.Println()

	for i := 0; i < MAX_PROCESSES; i++{
		//wait for the process to finish and return.
		procs[i].Wait()
		fmt.Println("Child Process " + strconv.Itoa(i) + " finished")
	}

	fmt.Println()
}

