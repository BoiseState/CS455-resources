CFLAGS = -g

.c.o:
	$(CC) -o $*.o -c $(CFLAGS) $< $(LFLAGS)

ALL = macro portable

all: $(ALL)

clean:
	$(RM) $(ALL) *.o data
