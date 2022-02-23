#
# @(#)Makefile	2.1 88/08/11 4.0 RPCSRC
#
CC=gcc
CFLAGS=-Wall -g -I/usr/include/tirpc
BIN = printmsg server remote_printmsg
GEN = msg_clnt.c msg_svc.c msg.h
LIB = -ltirpc
#RPCCOM = rpcgen -T
RPCCOM = rpcgen -C -a

all: $(BIN)

#
# This is the non-networked version of the program
#
printmsg: printmsg.o
	$(CC) -o $@ printmsg.o

#
# note: no xdr routines are generated here, due to this service's
#       use of basic data types.
#
$(GEN): msg.x
	$(RPCCOM) msg.x

server: server.o msg_svc.o
	$(CC) -o $@ server.o msg_svc.o $(LIB)

remote_printmsg: remote_printmsg.o msg_clnt.o
	$(CC) -o $@ remote_printmsg.o msg_clnt.o $(LIB)

remote_printmsg.o: remote_printmsg.c msg.h

server.o: server.c msg.h

clean cleanup:
	rm -f $(GEN) *.o $(BIN) core a.out msg_client.* msg_server.* Makefile.msg

