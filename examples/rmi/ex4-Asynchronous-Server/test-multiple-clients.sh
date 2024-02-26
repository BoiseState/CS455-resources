#!/bin/bash
# Run the server first and then run this and watch server output to check
# if RMI is multi-threaded or not.

case $# in
0) echo "Usage: "`basename $0` " <server hostname> [<registry port>]"; exit 1;;
esac
serverHost=$1
if test "$2" = ""
then
    registryPort=1099
else
    registryPort=$2
fi


sleep 2

echo "Starting four clients simultaneously"
java asynchronous.client.MyClient $serverHost $registryPort &
java asynchronous.client.MyClient $serverHost $registryPort &
java asynchronous.client.MyClient $serverHost $registryPort &
java asynchronous.client.MyClient $serverHost $registryPort &

