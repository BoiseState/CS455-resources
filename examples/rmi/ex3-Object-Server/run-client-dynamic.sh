#!/bin/bash

case $# in
0) echo "Usage: "`basename $0` " <server hostname>"; exit 1;;
esac
serverHost=$1

java -Djava.rmi.server.codebase="http://cs.boisestate.edu/~amit/teaching/455/rmi/ex3-Client-Callback/" synchronous.client.MyClient $serverHost

