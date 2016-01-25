CC=gcc
CFLAGS=-Wall -g
PROGS =	client multithreaded-server
CLEANFILES= *.o core a.out
LIBS=-lpthread -lnsl

all:	${PROGS}

square.h square_clnt.c square_svc.c square_xdr.c:	square.x
			rpcgen -a square.x

square_clnt.o: square_clnt.c square.h

square_svc.o: square_svc.c square.h

client:	square.h client.o square_clnt.o square_xdr.o 
			${CC} ${CFLAGS} -o $@ client.o square_clnt.o square_xdr.o  \
				${LIBS} ${LIBS_RPC}

multithreaded-server:	square.h server.o square_svc.o square_xdr.o 
			cp multithreaded_square_svc.c square_svc.c
			${CC} ${CFLAGS} -o $@ server.o square_svc.o square_xdr.o  \
				${LIBS} ${LIBS_RPC}

clean:
		/bin/rm -f ${PROGS} ${CLEANFILES} square_client.c square_server.c square_clnt.c\
		square_svc.c *_xdr.c square.h server client Makefile.square
