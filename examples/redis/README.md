

How to run (from command line)
==============================

Install redis on your Fedora Linux VM.

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

The example requires jar files in the `libs/` subfolder.  (or use Maven to automatically
download them). We are using the following versiona for connecting to Redis using Java:

```
commons-pool2-2.13.0.jar  jedis-7.3.0.jar  slf4j-api-2.0.17.jar  slf4j-simple-2.0.17.jar
```

To compile:

javac -cp .:./libs/*  RedisExample1.java

To run:

java -cp .:./libs/*  RedisExample1


You can then start up the redis client to play with the data store:

```
redis-cli
```

Try some commands such as `hgetall user-account1`
