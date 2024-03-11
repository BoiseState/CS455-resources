
This example will not work as onyx is no longer running a webserver!

The example LoadClient.java loads an object dynamically over the Internet.  It assumes that the
object client RunAway (compiled class in bytecode format) lives on an accessible directory on
another host that has a HTTPD server running on it. Try it out as follows:

```
java -Djava.security.manager -Djava.security.policy=mysecurity.policy LoadClient http://cscluster00.boisestate.edu/rmi/ex5-Load-Remote-Class/
```

The class `RunAway.class` lives on cscluster00 under 

```
http://cscluster00.boisestate.edu/rmi/ex5-Load-Remote-Class/
```

(don't forget to add the trailing slash).
