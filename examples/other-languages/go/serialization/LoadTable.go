//Author: Amit Jain & Paul Ellis
package main

import (
	"fmt"
	"image/color"
	"bytes"
	"encoding/gob"
	"os"
)

//Just check if an error was actually thrown
func CheckErr(e error){
	if e != nil {
		panic(e)
	}
}

func main(){
	//Buffer to hold the bytes for decoding
	var buffer bytes.Buffer

	//Decoding Object reading data from the buffer
	dec := gob.NewDecoder(&buffer)
	gob.Register(color.RGBA{})

	//Read the file and store the bytes in the byte slice dat
	dat, err := os.ReadFile("hash.ser")
	CheckErr(err)

	//Write the data from the slice into the buffer that the decoder
	//will read from
	buffer.Write(dat)

	//The data structure we want to fill with the decoding
	table := make(map[string]any)

	//Decode the table
	err = dec.Decode(&table)
	CheckErr(err)
	
	fmt.Println(table)

	//Ranged for loop will loop over all elements
	//in the map providing key value pairs
	for key, value := range table{
		fmt.Println(key, value)
	}

}