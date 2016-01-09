#!/bin/sh
# Spawns an RMI server

java -Djava.security.policy=mysecurity.policy RMIThreadServerImpl
