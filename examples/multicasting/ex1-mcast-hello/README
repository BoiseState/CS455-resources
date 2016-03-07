Run as follows (in the lab):

Pick two nodes (say node01 and node02):

On node01:

java Coordinator <numPackets> <network-interface>


On node02:

java Listener <network-interface>


Sample output:

[amit@node01 ex1-mcast-hello]$ java Coordinator 20  em1
Coordinator-- Hello:0     packet# 1 from /192.168.0.101
Coordinator-- Hello:1     packet# 2 from /192.168.0.101
Coordinator-- Hello:2     packet# 3 from /192.168.0.101
Coordinator-- Hello:3     packet# 4 from /192.168.0.101
Coordinator-- Hello:4     packet# 5 from /192.168.0.101
Coordinator-- Hello:5     packet# 6 from /192.168.0.101
Coordinator-- Hello:6     packet# 7 from /192.168.0.101
Coordinator-- Hello:7     packet# 8 from /192.168.0.101
Coordinator-- Hello:8     packet# 9 from /192.168.0.101
Coordinator-- Hello:9     packet# 10 from /192.168.0.101
Coordinator-- Hello:10     packet# 11 from /192.168.0.101
Coordinator-- Hello:11     packet# 12 from /192.168.0.101
Coordinator-- Hello:12     packet# 13 from /192.168.0.101
Coordinator-- Hello:13     packet# 14 from /192.168.0.101
Coordinator-- Hello:14     packet# 15 from /192.168.0.101
Coordinator-- Hello:15     packet# 16 from /192.168.0.101
Coordinator-- Hello:16     packet# 17 from /192.168.0.101
Coordinator-- Hello:17     packet# 18 from /192.168.0.101
Coordinator-- Hello:18     packet# 19 from /192.168.0.101
Coordinator-- Hello:19     packet# 20 from /192.168.0.101
[amit@node01 ex1-mcast-hello]$ 

[amit@node02 ex1-mcast-hello]$ java Listener em1
Listener-- Hello:2     packet# 1 from /192.168.0.101
Listener-- Hello:3     packet# 2 from /192.168.0.101
Listener-- Hello:4     packet# 3 from /192.168.0.101
Listener-- Hello:5     packet# 4 from /192.168.0.101
Listener-- Hello:6     packet# 5 from /192.168.0.101
Listener-- Hello:7     packet# 6 from /192.168.0.101
Listener-- Hello:8     packet# 7 from /192.168.0.101
Listener-- Hello:9     packet# 8 from /192.168.0.101
Listener-- Hello:10     packet# 9 from /192.168.0.101
Listener-- Hello:11     packet# 10 from /192.168.0.101
Listener-- Hello:12     packet# 11 from /192.168.0.101
Listener-- Hello:13     packet# 12 from /192.168.0.101
Listener-- Hello:14     packet# 13 from /192.168.0.101
Listener-- Hello:15     packet# 14 from /192.168.0.101
Listener-- Hello:16     packet# 15 from /192.168.0.101
Listener-- Hello:17     packet# 16 from /192.168.0.101
Listener-- Hello:18     packet# 17 from /192.168.0.101
Listener-- Hello:19     packet# 18 from /192.168.0.101
java.net.SocketTimeoutException: Receive timed out
[amit@node02 ex1-mcast-hello]$ 

