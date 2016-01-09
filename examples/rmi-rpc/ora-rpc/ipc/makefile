CFLAGS = -g

.c.o:
	$(CC) -o $*.o -c $(CFLAGS) $< $(LFLAGS)

ALL = childTimer day remote rexec status stest wtest

all:$(ALL)

clean:
	$(RM) $(ALL)
