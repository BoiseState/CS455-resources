LLA = multiAsyncRls_svc # server compatible w/ ../async client
ALL = $(LLA) \
		multiRls_svc	# server compatible w/ ../minimum/rls client

CFLAGS = -I../minimum

all: $(ALL)

multiAsyncRls_svc: multiAsyncRls_svc.c rls_xdr.o read_dir.o
	$(CC) $(CFLAGS) -o multiAsyncRls_svc multiAsyncRls_svc.c rls_xdr.o read_dir.o
	
multiRls_svc: multiRls_svc.c rls_xdr.o  read_dir.o
	$(CC) $(CFLAGS) -o multiRls_svc multiRls_svc.c rls_xdr.o read_dir.o

clean:
	$(RM) $(ALL) *.o
