#!/bin/bash
port=$1
if [ -z "$port" ]; then
  port=5005
fi
echo "port" $port
java -jar TcpEcho.jar client localhost $port "hello" 5 2000 &
java -jar TcpEcho.jar client localhost $port "nice to meet you" 5 2000 &
java -jar TcpEcho.jar client localhost $port "goodbye" 5 2000 &
wait
