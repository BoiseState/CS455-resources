ALL = asyncRls asyncRls_svc
CFLAGS = -I../minimum

all: $(ALL)

asyncRls: asyncRls.c rls_xdr.o 
	$(CC) -o asyncRls asyncRls.c rls_xdr.o
	
asyncRls_svc: asyncRls_svc.c rls_xdr.o  read_dir.o
	$(CC) -o asyncRls_svc asyncRls_svc.c rls_xdr.o read_dir.o

clean:
	$(RM) $(ALL) *.o
