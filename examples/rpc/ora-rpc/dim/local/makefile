.c.o:
	$(CC) -o $*.o -c $(CFLAGS) $< $(LFLAGS)

im: im.o im_proc.o
	$(CC) -o im $(CFLAGS) $(LFLAGS) im.o im_proc.o

clean:
	$(RM) *.o im
