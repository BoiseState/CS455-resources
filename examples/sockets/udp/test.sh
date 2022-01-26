#!/bin/bash

server=localhost
cd ..

java udp.UdpClient1 $server 5005 &
java udp.UdpClient1 $server 5005 &
java udp.UdpClient1 $server 5005 &
java udp.UdpClient1 $server 5005 &
java udp.UdpClient1 $server 5005 &
java udp.UdpClient1 $server 5005 &
java udp.UdpClient1 $server 5005 &
java udp.UdpClient1 $server 5005 &

wait
