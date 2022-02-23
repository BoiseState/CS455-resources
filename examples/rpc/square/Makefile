CC=gcc
CFLAGS=-Wall -g  -I/usr/include/tirpc 
LIBS=-ltirpc
PROGS =	client server client_test
CLEANFILES= *.o core a.out

all:	${PROGS}

square.h square_clnt.c square_svc.c square_xdr.c:	square.x
			rpcgen -C square.x

square_clnt.o: square_clnt.c square.h

square_svc.o: square_svc.c square.h

client:	square.h client.o square_clnt.o square_xdr.o error.o
			${CC} ${CFLAGS} -o $@ client.o square_clnt.o square_xdr.o error.o \
				${LIBS} ${LIBS_RPC}
client_test:	square.h client_test.o square_clnt.o square_xdr.o error.o
			${CC} ${CFLAGS} -o $@ client_test.o square_clnt.o square_xdr.o error.o \
				${LIBS} ${LIBS_RPC}

server:	square.h server.o square_svc.o square_xdr.o error.o
			${CC} ${CFLAGS} -o $@ server.o square_svc.o square_xdr.o  error.o\
				${LIBS} ${LIBS_RPC}

clean:
		rm -f ${PROGS} ${CLEANFILES} *_clnt.c *_svc.c *_xdr.c square.h
