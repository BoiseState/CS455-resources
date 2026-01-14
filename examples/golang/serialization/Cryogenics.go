//Author: Amit Jain & Paul Ellis

package main

import (
	"fmt"
	"bytes"
	"encoding/gob"
	"os"
	"strings"
)

func CheckErr(e error){
	if e != nil{
		panic(e)
	}
}

func FreezeDry(){
	//See SaveTable.go for details on what is going on here
	m := NewMioAlma("Marco Polo", 123456789)
	fmt.Println("Serializing object: ", m)

	var buffer bytes.Buffer
	gob.Register(MioAlma{})

	enc := gob.NewEncoder(&buffer)
	err := enc.Encode(m)
	CheckErr(err)

	err = os.WriteFile("mio.ser", buffer.Bytes(), 0644)
	CheckErr(err)
}

func Revive(){
	//See LoadTable.go for more details on what is happening here
	var buffer bytes.Buffer

	dec := gob.NewDecoder(&buffer)
	gob.Register(MioAlma{})

	data, err := os.ReadFile("mio.ser")
	CheckErr(err)

	buffer.Write(data)

	var mio MioAlma
	err = dec.Decode(&mio)

	CheckErr(err)
	fmt.Println("Deserialized object: ", mio)
}

func main(){
	args := os.Args[1:]

	if len(args) != 1 {
		fmt.Println("Usage: Cryogenics <freeze|revive>")
		os.Exit(1)
	}

	//strings.EqualFold(str1, str2) will compare the strings ignoring case
	if strings.EqualFold(args[0], "freeze") {
		FreezeDry()
	} else if strings.EqualFold(args[0], "revive"){
		Revive()
	}
}