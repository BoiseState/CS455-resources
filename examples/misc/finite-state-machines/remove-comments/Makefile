CC = gcc
CFLAGS = -Wall -g
PROGS=remove-comments-v1 remove-comments-v2 test1

all: $(PROGS)

remove-comments-v1: remove-comments-v1.o
	$(CC) $(CFLAGS) $^ -o $@

remove-comments-v2: remove-comments-v2.o
	$(CC) $(CFLAGS) $^ -o $@

test1: test1.o
	echo "The following program intentionally generates warnings during compilation!"
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o $(OBJS) $(PROGS)
