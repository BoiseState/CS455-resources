#!/bin/bash
# Run the server first and then run this and watch server output to check
# if RMI is multi-threaded or not.

case $# in
0|1|2) echo "Usage: "`basename $0` " <server hostname> <registry port> <#clients>"; exit 1;;
esac

serverHost=$1
registryPort=$2
n=$3


sleep 2

echo
echo "Starting $n clients simultaneously"
echo

for i in $(seq 1 $n)
do
java  synchronous.client.MyClient $serverHost $registryPort &
done

echo
echo "Waiting for the clients to finish"
echo

wait
