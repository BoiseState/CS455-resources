#!/bin/sh

case $# in
0) registryPort=1099;;
1) registryPort=$1;;
esac

echo
echo "Killing existing rmiregistry"
killall -9 rmiregistry >& /dev/null

echo
echo "Starting new rmiregistry from $(pwd) on port $registryPort"
echo
export CLASSPATH=$(pwd):$CLASSPATH

rmiregistry $registryPort >& /dev/null &
if test "$?" -ne 0
then
  echo "Failed to start rmiregistry"
  exit 1
fi
sleep 2

echo
echo "Starting server clientcallback.server.MyServer with registry port $registryPort"
java  asynchronous.server.MyServer $registryPort
echo

