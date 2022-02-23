
CC=gcc
CFLAGS=-Wall -g  -I/usr/include/tirpc
LIBS=-ltirpc

all: client linkedlist_server


client: llclient.o ll_clnt.o ll_xdr.o
	$(CC) -o client llclient.o ll_clnt.o ll_xdr.o $(LIBS)


linkedlist_server: llservice.o ll_svc.o ll_xdr.o
	$(CC) -o linkedlist_server llservice.o ll_svc.o ll_xdr.o $(LIBS)

ll.h: ll.x
	rpcgen -C ll.x

clean: 
	rm -f *.o ll.h  ll_clnt.c ll_svc.c ll_xdr.c client linkedlist_server

llclient.c: ll.h
llservice.c: ll.h

ll_clnt.o: ll_clnt.c ll.h
	$(CC) $(CFLAGS) -c ll_clnt.c

ll_xdr.o: ll_xdr.c ll.h
	$(CC) $(CFLAGS) -c ll_xdr.c


ll_svc.o: ll_svc.c ll.h
	$(CC) $(CFLAGS) -c ll_svc.c

