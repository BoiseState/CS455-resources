#!/bin/bash


case $# in
0) echo "Usage: "`basename $0` " <server hostname> <registry port>"; exit 1;;
1) echo "Usage: "`basename $0` " <server hostname> <registry port>"; exit 1;;
esac

serverHost=$1
registryPort=$2
if test "$registryPort" = ""
then
	registryPort=1099
else
	registryPort=$2
fi

echo "Running client with server host: $serverHost and registry port: $registryPort"

java asynchronous.client.MyClient $serverHost $registryPort

