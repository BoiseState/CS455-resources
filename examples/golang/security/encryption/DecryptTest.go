package main

import (
	"crypto/aes"
	"crypto/cipher"
	"fmt"
	"os"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {
	//Open the encrypted file and defer the closing of it until the end of the function
	dataFile, err := os.Open("./data.encrypted")
	check(err)
	defer dataFile.Close()

	//Open the key file and defer the closing of it until the end of the function
	fis, err := os.Open("./key")
	check(err)
	defer fis.Close()

	//Read the key from the file
	keyBytes := make([]byte, 32)
	_, err = fis.Read(keyBytes)
	fmt.Println("Key: ", string(keyBytes))

	//Read the encrypted data from the file
	enc := make([]byte, 4096)
	numRead, err := dataFile.Read(enc)
	fmt.Println("Read ", numRead, " bytes")
	check(err)
	enc = enc[:numRead]

	//Create the cipher block from the key
	block, err := aes.NewCipher(keyBytes)
	check(err)

	//Create the GCM cipher from the cipher block
	aesGCM, err := cipher.NewGCM(block)
	check(err)

	//Get the nonce size from the GCM cipher
	nonceSize := aesGCM.NonceSize()
	//Get the nonce from the encrypted data and the ciphertext
	nonce, ciphertext := enc[:nonceSize], enc[nonceSize:]

	//Decrypt the data
	plaintext, err := aesGCM.Open(nil, nonce, ciphertext, nil)
	check(err)
	fmt.Print("Decrypted data: ")
	fmt.Println(string(plaintext))
}
