

The server has a global counter, and whenever clients read() it, they should
always get a zero.  The server has a function, update(), which increments the
counter to a large number, then decrements it back to zero.  The update()
function should appear atomic.

A client simply calls read(), update(), read(), and reports the results of
both read() calls.  If anything other than zero is reported, then we have a
case of data inconsistency.

If read() and update() are declared synchronized, then things work as
expected, and only one client at a time can access the counter variable for
either read()ing or update()ing.  If either function is not synchronized, bad
things happen, and clients report wildly varying values for "counter".


Usage:

```
vim RMIThreadServerImpl.java	<-- change read() and update() declarations
make
rmiregistry 5130 &
./runserver.sh &
./test.sh			<-- spawns 10 RMI clients simultaneously

...

fg & kill runserver.sh
fg & kill rmiregistry 5130
```


NOTE: This example assumes that the rmiregistry is running on port 1099. If that isn't the case,
change the code in the server and client.

Paul Kreiner
Amit Jain
