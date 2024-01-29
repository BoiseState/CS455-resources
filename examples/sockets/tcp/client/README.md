

# TCP Client Examples

## DateAtHost example

Connect to a server that provides daytime service to obtain time and date on the server.
This will only work if you have installed and configured the xinetd daemon that provides the
daytime service at port 37. This is usually disabled for security purposes.

The tcp folder and  subfolders are packages, so run the programs from one level above this folder

```
cd ../
java tcp.client.DateAtHost localhost
```


## PortScanner

A simple port scanner that attempts to connect to a server at every port in the given range. A
white hat security tool! For example, try the following:

```
cd ../
java tcp.client.PortScanner localhost 1 65535 
```




