
CC=gcc
CFLAGS=-Wall -I.

all: oobsend oobrecv

oobsend: oobsend.o error.o
	$(CC) $(CFLAGS) -o $@  oobsend.o error.o

oobrecv: oobrecv.o error.o
	$(CC) $(CFLAGS) -o $@  oobrecv.o error.o

clean:
	/bin/rm -f *.o core a.out oobsend oobrecv
