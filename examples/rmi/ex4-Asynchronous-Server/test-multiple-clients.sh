#!/bin/bash
# Run the server first and then run this and watch server output to check
# if RMI is multi-threaded or not.

case $# in
0) echo "Usage: "`basename $0` " <server hostname>"; exit 1;;
esac
serverHost=$1

sleep 2
java -Djava.security.policy=mysecurity.policy callback.client.MyClient $serverHost &
java -Djava.security.policy=mysecurity.policy callback.client.MyClient $serverHost &
java -Djava.security.policy=mysecurity.policy callback.client.MyClient $serverHost &
java -Djava.security.policy=mysecurity.policy callback.client.MyClient $serverHost &
java -Djava.security.policy=mysecurity.policy callback.client.MyClient $serverHost &

