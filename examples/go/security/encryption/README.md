# AES Encryption in Go
## Run the example
In order to run the example use the following commands:

> `go run EncryptTest.go`
>
> `go run DecryptTest.go`

## Thoughts
Because go is a less object oriented language than Java you will notice that the key is generated
into a byte slice rather than an object. We can write this out to a file on the computer and change the
files properties to only allow the owner to read the key.
The rest of the encryption and decryption is different from Java due to the explicit use of the 
GCM and Nonce. If you would like to read more on this please refer to the Go Documentation or
this [wikepedia article](https://en.wikipedia.org/wiki/Galois/Counter_Mode)