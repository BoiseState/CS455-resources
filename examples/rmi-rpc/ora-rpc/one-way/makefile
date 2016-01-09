# this is probably done just as well in a shell script, but here goes...
ALL = one-way one-way_svc batch broadcast
RPCGEN = rpcgen

all: $(ALL)


one-way one-way_svc one-way_clnt.c: one-way.x one-way.c
	$(RPCGEN) one-way.x
	$(RPCGEN) -s tcp -o one-way_svc.c one-way.x
	sed 's/static struct timeval TIMEOUT = { 25, 0 }/extern struct timeval TIMEOUT/' \
	< one-way_clnt.c > /tmp/$$; mv /tmp/$$ one-way_clnt.c
	make -f ../makefile.template APPN=one-way one-way one-way_svc

batch_clnt.c: one-way_clnt.c
	sed 's/xdr_void/NULL/' < one-way_clnt.c > batch_clnt.c

batch: one-way_xdr.o batch_clnt.c one-way.c
	$(CC) $(CFLAGS) -o batch one-way_xdr.o batch_clnt.c one-way.c

clean:
	$(RM) one-way one-way_svc one-way_svc.c one-way_xdr.c one-way_clnt.c \
	one-way.h batch_clnt.c *.o *.*% $(ALL) one-way_shared.h
