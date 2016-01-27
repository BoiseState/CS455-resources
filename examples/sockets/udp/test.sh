#!/bin/bash

cd ..

java udp.UdpClient1 localhost 5005 &
java udp.UdpClient1 localhost 5005 &
java udp.UdpClient1 localhost 5005 &
java udp.UdpClient1 localhost 5005 &
java udp.UdpClient1 localhost 5005 &
java udp.UdpClient1 localhost 5005 &
java udp.UdpClient1 localhost 5005 &
java udp.UdpClient1 localhost 5005 &

wait
