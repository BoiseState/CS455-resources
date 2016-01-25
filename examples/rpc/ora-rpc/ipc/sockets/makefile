# As discussed in _Power Programming with RPC_, su /etc/services entries
# or mods to the client/server pairs is required.

A = TCP_AF_INETclient TCP_AF_INETserver # c/s pair for Internet addressing, TCP
B = TCP_AF_UNIXclient TCP_AF_UNIXserver # c/s pair for Unix domain addressing, TCP
C = UDP_AF_INETclient UDP_AF_INETserver # c/s pair for Internet addressing, TCP
ALL = $(A) $(B) $(C) Loop_TCP_AF_INETserver # a crude fix to make server run forever

CFLAGS = -g

.c.o:
	$(CC) -o $*.o -c $(CFLAGS) $< $(LFLAGS)
.c :
	$(CC) -o $* $(CFLAGS) $< $(LFLAGS) read_dir.c

all: $(ALL)

clean:
	$(RM) $(ALL) *.o
