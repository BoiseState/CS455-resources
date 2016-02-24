.c.o:
	gcc -c -Wall -g $<

all: client userlookup_server

COBJ = userlookup_clnt.o userlookup_xdr.o userlookup_client.o

SOBJ = userlookup_svc.o userlookup_xdr.o userlookup_services.o

client: $(COBJ)
	gcc -o client $(COBJ)


userlookup_server: $(SOBJ)
	gcc -o userlookup_server $(SOBJ)


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

