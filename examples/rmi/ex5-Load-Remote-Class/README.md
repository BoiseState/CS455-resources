
The example LoadClient.java loads an object dynamically over the Internet.  It assumes that the
object client RunAway (compiled class in bytecode format) lives on an accessible directory on
another host that has a HTTPD server running on it. Try it out as follows:

```
java LoadClient http://onyx.boisestate.edu/~amit/rmi/ex5-Load-Remote-Class/
```

The class `RunAway.class` lives on onyx under 

```
http://onyx.boisestate.edu/~amit/rmi/ex5-Load-Remote-Class/
```

(don't forget to add the trailing slash).
