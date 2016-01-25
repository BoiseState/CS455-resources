#!/bin/sh

killall -9 rmiregistry
export CLASSPATH=`pwd`:$CLASSPATH
rmiregistry &

sleep 2

#java -Djava.rmi.server.codebase='http://onyx.boisestate.edu/~amit/rmi/ex2/' -Djava.security.policy=mysecurity.policy  MyServer

java  -Djava.security.policy=mysecurity.policy  asynchronous.server.MyServer



