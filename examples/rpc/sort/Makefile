#
# @(#)Makefile	2.1 88/08/11 4.0 RPCSRC
#
CC=gcc
CFLAGS=-Wall -g -I/usr/include/tirpc
BIN =  remote_sort sort_server
GEN = sort_clnt.c sort_svc.c sort_xdr.c sort.h
LIB = -ltirpc
RPCCOM = rpcgen

all: $(BIN)

remote_sort: remote_sort.o sort_clnt.o sort_xdr.o
	$(CC) $(LDFLAGS) -o $@ remote_sort.o sort_clnt.o sort_xdr.o $(LIB)

remote_sort.o: remote_sort.c sort.h

sort_clnt.c:
	$(RPCCOM) -l sort.x >$@

sort_server: sort_server.o sort_svc.o sort_xdr.o
	$(CC) $(LDFLAGS) -o $@ sort_server.o sort_svc.o sort_xdr.o $(LIB)

sort_server.o: sort_server.c sort.h

sort_svc.c:
	$(RPCCOM) -s udp sort.x >$@

sort_xdr.c:
	$(RPCCOM) -c sort.x >$@

sort.h:
	$(RPCCOM) -h sort.x >$@

clean cleanup:
	rm -f $(GEN) *.o $(BIN)

