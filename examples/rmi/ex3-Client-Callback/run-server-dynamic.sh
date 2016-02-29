#!/bin/sh

echo "Killing existing rmiregistry"
killall -9 rmiregistry >& /dev/null

echo "Starting new rmiregistry from `pwd`"
export CLASSPATH=`pwd`:$CLASSPATH

# Make sure rmiregistry doesn't pick up local classes
currdir=`pwd`
cd /
rmiregistry &
cd $currdir

echo "Starting server clientcallback.server.MyServer"
# Use this for dynamically loading classes over the network
java -Djava.rmi.server.codebase='http://onyx.boisestate.edu/~amit/rmi/ex3-Client-Callback/' -Djava.security.policy=mysecurity.policy  callback.server.MyServer




