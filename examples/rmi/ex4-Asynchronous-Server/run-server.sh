#!/bin/sh

echo
echo "Stopping existing rmiregistry, if any"
killall -9 rmiregistry


echo
echo "Starting new regitsry from folder: "  $(pwd)
export CLASSPATH=`pwd`:$CLASSPATH
rmiregistry &

sleep 1

#java -Djava.rmi.server.codebase='http://onyx.boisestate.edu/~amit/rmi/ex2/' -Djava.security.policy=mysecurity.policy  MyServer

echo
echo "Starting server"
echo
java  -Djava.security.policy=mysecurity.policy  asynchronous.server.MyServer



