

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

The example requires jar files in the `libs/` subfolder. To compile:

javac -cp .:./libs/*  RedisExample1.java

To run:

java -cp .:./libs/*  RedisExample1


You can then start up the redis client to play with the data store:

```
redis-cli
```

Try some commands such as `hgetall user-account1`
