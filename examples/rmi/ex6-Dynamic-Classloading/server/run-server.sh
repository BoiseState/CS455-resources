#!/bin/sh

echo "Killing existing rmiregistry"
killall -9 rmiregistry >& /dev/null


echo "Starting rmiregistry"
currdir=`pwd`
cd /
rmiregistry &
cd $currdir

echo "Starting server"

java -Djava.rmi.server.codebase="http://onyx.boisestate.edu/~amit/rmi/ex6-Dynamic-Classloading/" -Djava.security.policy=mysecurity.policy  MyServer


