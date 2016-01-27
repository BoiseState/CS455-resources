#!/bin/bash

cd ../..
java tcp.multithreaded.TimeClient  localhost 5005 &
java tcp.multithreaded.TimeClient  localhost 5005 &
java tcp.multithreaded.TimeClient  localhost 5005 &
java tcp.multithreaded.TimeClient  localhost 5005 &
java tcp.multithreaded.TimeClient  localhost 5005 &
java tcp.multithreaded.TimeClient  localhost 5005 &
java tcp.multithreaded.TimeClient  localhost 5005 &
java tcp.multithreaded.TimeClient  localhost 5005 &

wait

