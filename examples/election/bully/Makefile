CC = gcc
CFLAGS =  -g -O2 -Wall 

CLEANFILES = core core.* *.core *.o temp.* *.out typescript* \
		*.lc *.lh *.bsdi *.sparc *.uw


PROGS =	bully

all:	${PROGS}

bully:	Elect.c unix.c
		${CC} ${CFLAGS} -o $@ Elect.c unix.c

clean:
		rm -f ${PROGS} ${CLEANFILES}
