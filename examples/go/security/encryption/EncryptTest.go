package main

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"fmt"
	"io"
	"os"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {
	fos, err := os.Create("./key")
	check(err)
	defer fos.Close()

	dataFile, err := os.Create("./data.encrypted")
	check(err)
	defer dataFile.Close()

	keyBytes := make([]byte, 32)
	fmt.Println("Creating AES key")
	//Read random bytes into the keyBytes slice
	_, err = rand.Read(keyBytes)
	check(err)

	fmt.Println("Key: ", string(keyBytes))
	fos.Write(keyBytes)
	//We can change the read/write/execute permissions of a file using standard
	//chmod numbering. This change will make the file read only for the owner
	//Think of the number in binary each digit being 3 bits (read,write,execute)
	//Because we only want the owner to be able to read the file we set the first
	//digit to 0b100 (decimal is 4) and the rest to 0
	fos.Chmod(400)

	plaintext := []byte("Hello World!")
	fmt.Println("Plaintext: ", string(plaintext))

	//Create the cipher block from the key
	cipherBlock, err := aes.NewCipher(keyBytes)
	check(err)

	//Create the GCM cipher from the cipher block
	aesGCM, err := cipher.NewGCM(cipherBlock)
	check(err)

	//Create a nonce the same size as the GCM cipher's nonce size
	nonce := make([]byte, aesGCM.NonceSize())
	//Read random bytes into the nonce slice
	_, err = io.ReadFull(rand.Reader, nonce)
	check(err)

	//Encrypt the data
	ciphertext := aesGCM.Seal(nonce, nonce, plaintext, nil)

	fmt.Print("Encrypted data: ")
	fmt.Println(string(ciphertext))
	fmt.Println("Writing encrypted data to file: data.encrypted")
	//Write the encrypted data to the file
	dataFile.Write(ciphertext)
}
