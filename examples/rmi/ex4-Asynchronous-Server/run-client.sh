#!/bin/bash

case $# in
0) echo "Usage: "`basename $0` " <server hostname> <registry port>"; exit 1;;
1) echo "Usage: "`basename $0` " <server hostname> <registry port>"; exit 1;;
esac

serverHost=$1
if test "$2" = ""
then
	registry=1099
else
	registryPort=$2
fi

java asynchronous.client.MyClient $serverHost $registryPort

