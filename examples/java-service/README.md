
Compile classes using make.

Add the entry 

rmi-date-server  5005/tcp

at the end of /etc/services file

Make sure xinetd is installed on your system. If not, install it using:

dnf install xinetd

Copy the file rmi-date-server to /etc/xinetd.d/

Reload xinetd (as superuser)

su
/sbin/service xinetd reload
exit

Now run the client:

java Client localhost 5005 hello

After running the client, the server is left running. Kill it.

ps augx | grep RMIDateServer

kill <process id>


