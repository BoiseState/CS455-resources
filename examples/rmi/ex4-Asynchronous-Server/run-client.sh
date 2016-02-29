#!/bin/bash

case $# in
0) echo "Usage: "`basename $0` " <server hostname>"; exit 1;;
esac

serverHost=$1

java -Djava.security.policy=mysecurity.policy asynchronous.client.MyClient $serverHost

