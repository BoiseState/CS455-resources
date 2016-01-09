#!/bin/bash
# Run the server first and then run this and watch server output to check
# if RMI is multi-threaded or not.

sleep 2
java -Djava.security.policy=mysecurity.policy callback.client.MyClient localhost &
java -Djava.security.policy=mysecurity.policy callback.client.MyClient localhost &
java -Djava.security.policy=mysecurity.policy callback.client.MyClient localhost &
java -Djava.security.policy=mysecurity.policy callback.client.MyClient localhost &
java -Djava.security.policy=mysecurity.policy callback.client.MyClient localhost &
