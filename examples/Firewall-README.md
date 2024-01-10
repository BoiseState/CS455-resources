
Running a TCP based application across a firewall using SSH tunneling
======================================================================


Suppose you are running a client on your local system and you want to
connect to a server that you are running on onyx.boisestate.edu. Then
connect to onyx using ssh with the tunneling option.

ssh -L5005:onyx.boisestate.edu:5005 onyx.boisestate.edu

which forwards all traffic to port 5005 on your local machine to port
5005 on onyx. This also logs you on to onyx, where you can then start
your server. You can also use this to change the port. Once this is setup,
then run your client as before except specifying localhost as the hostname
for the server.


Running a UDP based application across a firewall using SSH tunneling
======================================================================

ssh -L5005:onyx.boisestate.edu:5005 onyx.boisestate.edu

Server side: socat tcp4-listen:5005,reuseaddr,fork UDP:nameserver:5005 

java Server &


Client side: socat udp4-listen:5005,reuseaddr,fork tcp:localhost:5005

java Client localhost

If you don't have the socat program, install it using (replace yum with apt-get or dnf for
Ubuntu or Fedora)

su
yum install socat
exit


