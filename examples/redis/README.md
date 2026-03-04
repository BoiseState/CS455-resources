

How to run (from command line)
==============================

Install redis on your Linux VM. For Fedora:

```
dnf install redis redis-devel
```

(or use apt-get for Ubuntu)

In a separate terminal, start up the redis server with the command

```
redis-server
```

A quick test for the server being up, use the redis command line client:

```
redis-cli ping
should reply-->PONG
```

Using the Redis command line client
===================================

You can then start up the redis client to play with the data store:

```
redis-cli
```

Sample session:

```
touch redis.conf
redis-server redis.conf

--> in a separate terminal

redis-cli

Using strings
=============

127.0.0.1:6379> set key1 "Howdy"
OK
127.0.0.1:6379> mset key2 "Hello" key3 "World"
OK
127.0.0.1:6379> append key4 "Bonjour"
(integer) 7
127.0.0.1:6379> get key4
"Bonjour"
127.0.0.1:6379> mget key1 key2 key3
1) "Howdy"
2) "Hello"
3) "World"
127.0.0.1:6379> set keyNum 1
OK
127.0.0.1:6379> incr keyNum
(integer) 2
127.0.0.1:6379> incrby keyNum 3
(integer) 5
127.0.0.1:6379> decr keyNum
(integer) 4
127.0.0.1:6379> decrby keyNum 3
(integer) 1
127.0.0.1:6379> append keyNum "111"
(integer) 4
127.0.0.1:6379> get keyNum
"1111"
127.0.0.1:6379> append key1 ", Woody"
(integer) 12
127.0.0.1:6379> get key1
"Howdy, Woody"
127.0.0.1:6379> 


Using lists
===========

127.0.0.1:6379> lpush key_philosophy1 "absentminded"
127.0.0.1:6379> lpush key_philosophy1 "am"
127.0.0.1:6379> lpush key_philosophy1 "I"
127.0.0.1:6379> lpush key_philosophy1 "therefore"
127.0.0.1:6379> lpush key_philosophy1 "think"
127.0.0.1:6379> lpush key_philosophy1 "I"
Output:
(integer) 5


rpush key_philosophy1 " - " " anonymous"
(integer) 7

127.0.0.1:6379> rpushx key_philosophy2 "This" "is" "a" "test"
(integer) 0

127.0.0.1:6379> lset key_philosophy1 5 "lost"
OK

127.0.0.1:6379> lrange key_philosophy1 0 7
1) "I"
2) "think"
3) "therefore"
4) "I"
5) "am"
6) "absentminded"
7) " - "
8) " anonymous"


lindex key_philosophy1 4    
"am"

llen key_philosophy1
(integer) 8

127.0.0.1:6379> lrem key_philosophy1 1 "I"
(integer) 1
127.0.0.1:6379> lrange key_philosophy1 0 7
1) "think"
2) "therefore"
3) "I"
4) "am"
5) "absentminded"
6) " - "
7) " anonymous"

127.0.0.1:6379> lpop key_philosophy1
"think"
127.0.0.1:6379> rpop key_philosophy1
" anonymous"

--> moving the last element of key_philosophy1 to the front of newList
127.0.0.1:6379> rpoplpush key_philosophy1 newList
" - "
127.0.0.1:6379> 


Using hashes
===========

--> creating a hash for the poet Emily Dickinson

127.0.0.1:6379> hset poet:Dickinson nationality American
(integer) 1

127.0.0.1:6379> hset poet:Dickinson nationality USA
(integer) 0

--> We can use hsetnx to set the value only if the key does not exist

127.0.0.1:6379> hsetnx poet:Dickinson nationality USA
(integer) 0

127.0.0.1:6379> hmset poet:Dickinson born 1830 died 1886 genre Lyrical 
OK

--> retrieving information from the hash


127.0.0.1:6379> hexists poet:Emerson
(integer) 0


127.0.0.1:6379> hget poet:Dickinson nationality
"American"

127.0.0.1:6379> hget poet:Dickinson born died
1) "1830"
2) "1886"

127.0.0.1:6379> hget poet:Emerson born died        
1) (nil)
2) (nil)

127.0.1:6379> hkeys poet:Dickinson
1) "nationality"
2) "born"
3) "died"   
4) "genre"

127.0.1:6379> hvals poet:Dickinson
1) "American"
2) "1830"
3) "1886"
4) "Lyrical"

127.0.1:6379> hgetall poet:Dickinson
1) "nationality"
2) "American"
3) "born"
4) "1830"
5) "died"
6) "1886"
7) "genre"
8) "Lyrical"

127 0.0.1:6379> hlen poet:Dickinson 
(integer) 4


127.0.0.1:6379> hstrlen poet:Dickinson nationality
(integer) 8


127.0.0.1:6379> hdel poet:Dickinson born died
(integer) 2

```


Using Redis from a Java application
===================================

Use the Jedis library to connect to Redis from Java.  See the `RedisExample1.java` file for an
example of how to do this.

The example requires jar files in the `libs/` subfolder.  (or use Maven to automatically
download them). We are using the following versiona for connecting to Redis using Java:

```
commons-pool2-2.13.0.jar  jedis-7.3.0.jar  slf4j-api-2.0.17.jar  slf4j-simple-2.0.17.jar
```

To compile:

javac -cp .:./libs/*  RedisExample1.java

To run:

java -cp .:./libs/*  RedisExample1

Then connect via the redis-cli and check the data that was inserted by the Java application.

For example: try some commands such as `hgetall user-account1`

For using Jackson library for JSON objetcs, sticl to version 2.21 (avoid the latest versions for now)
