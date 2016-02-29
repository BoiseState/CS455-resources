#!/bin/sh
# Spawns 10 RMI client processes, all trying to hit the RMI server
# simultaneously.

for i in 0 1 2 3 4 5 6 7 8 9
do
  ./run-client.sh 2>&1
done

