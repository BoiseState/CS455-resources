//Author: Amit Jain & Paul Ellis
package main

import (
	"math"
	"image/color"
	"bytes"
	"encoding/gob"
	"os"
)

func CheckErr(e error){
	if e != nil{
		panic(e)
	}
}

func main(){
	pi := math.Pi
	var col color.Color
	col = color.RGBA{255,0,0,0}
	table := make(map[string]any)

	table["string"] = "Marco Polo"
	table["int"] = 1619
	table["float64"] = pi
	table["color"] = col

	var buffer bytes.Buffer

	//If you use any interfaces/structs it's best to register them
	//to the gob so that it can encode correctly.
	gob.Register(color.RGBA{})

	//Create a new encoder that will output 
	//the encoding to the buffer
	enc := gob.NewEncoder(&buffer)

	//Encode the data from table into the buffer
	err := enc.Encode(table)
	CheckErr(err)

	//Write out the buffer's byte slice to hash.ser file. 
	//Uses the last argument to set R/W/E permission
	//This is a one and done file write so there is no need
	//to flush or close a writer
	err = os.WriteFile("hash.ser", buffer.Bytes(), 0644)
	CheckErr(err)

}