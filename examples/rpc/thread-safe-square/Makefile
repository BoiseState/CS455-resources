
PROGS =	client server
CFLAGS = -Wall -DDEBUG -I/usr/include/tirpc
LIBS_RPC= -ltirpc
CC=gcc

all:	${PROGS}

# On Solaris -A option automatically starts each server invocation as a
# separate thread. The option -M is to generate multi-thread safe code
square.h square_clnt.c square_svc.c square_xdr.c:	square.x
			rpcgen -C -M  square.x

square_clnt.o: square_clnt.c square.h

square_svc.o: square_svc.c square.h

client:	square.h client.o square_clnt.o square_xdr.o error.o
			${CC} ${CFLAGS} -o $@ client.o square_clnt.o square_xdr.o error.o \
				${LIBS} ${LIBS_RPC}

server:	square.h server.o square_svc.o square_xdr.o error.o
			${CC} ${CFLAGS} -o $@ server.o square_svc.o square_xdr.o error.o  \
				${LIBS} ${LIBS_RPC}

clean:
		rm -f *.o ${PROGS} ${CLEANFILES} *_clnt.c *_svc.c *_xdr.c square.h
