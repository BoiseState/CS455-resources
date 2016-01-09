ALL = lls rls rls_svc tcpRls

all: $(ALL)

lls: lls.c read_dir.o
	$(CC) -o lls lls.c read_dir.o

rls: rls.c rls_xdr.o 
	$(CC) -o rls rls.c rls_xdr.o
	
rls_svc: rls_svc.c rls_xdr.o  read_dir.o
	$(CC) -g -o rls_svc rls_svc.c rls_xdr.o read_dir.o

tcpRls: tcpRls.c rls_xdr.o
	$(CC) -g -o tcpRls tcpRls.c rls_xdr.o

clean:
	$(RM) $(ALL) *.o
