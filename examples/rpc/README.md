
RPC Examples in C
==================


Most likely, you will need to install rpcgen, libtirpc and libtirpc-devel packages. On Fedora
Linux, use the following command:

sudo dnf install rpcgen libtirpc libtirpc-devel


You will most likely need to add the following line to /etc/sysconfig/rpcbind

RPCBIND_ARGS="-i"

and then restart the rpcbind service

sudo /sbin/systemctl restart rpcbind 

The rpcbind service is used by all RPC servers.


