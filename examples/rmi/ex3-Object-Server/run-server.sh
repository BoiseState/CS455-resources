#!/bin/sh

case $# in
0) registryPort=1099;;
1) registryPort=$1;;
esac

echo
echo "Killing existing rmiregistry"
killall -9 rmiregistry >& /dev/null

echo
echo "Starting new rmiregistry from $(pwd)"
export CLASSPATH=$(pwd):$CLASSPATH

rmiregistry &

echo
echo "Starting server synchronous.server.MyServer"
java  -Djava.security.policy=mysecurity.policy  synchronous.server.MyServer $registryPort
echo

