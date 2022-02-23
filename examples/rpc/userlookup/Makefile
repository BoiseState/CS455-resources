
CC= gcc
CFLAGS= -Wall -I/usr/include/tirpc
LFLAGS= -ltirpc

.c.o:
	$(CC) -c $(CFLAGS) $<

all: client userlookup_server

COBJ = userlookup_clnt.o userlookup_xdr.o userlookup_client.o

SOBJ = userlookup_svc.o userlookup_xdr.o userlookup_services.o

client: $(COBJ)
	$(CC) -o client $(COBJ) $(LFLAGS)


userlookup_server: $(SOBJ)
	$(CC) -o userlookup_server $(SOBJ) $(LFLAGS)


userlookup.h: userlookup.x
	rpcgen -C userlookup.x

userlookup_clnt.c: userlookup.x
	rpcgen -C userlookup.x

userlookup_svc.c: userlookup.x
	rpcgen -C userlookup.x

userlookup_xdr.c: userlookup.x
	rpcgen -C userlookup.x

clean: 
	rm -f  *.o userlookup.h userlookup_clnt.c userlookup_svc.c \
	 userlookup_xdr.* client userlookup_server

