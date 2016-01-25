APPN = dim
#AUTH=	tell me what kind of authentication should be inserted, UNIX or DES
AUTH=NONE
CFLAGS= -g -DDEBUG
LFLAGS=

include ../../../makefile.template

# compile the protocol, add requested authentication and externs to the server
$(APPN)_xdr.c $(APPN)_svc.c $(APPN)_clnt.c $(APPN).h:: $(APPN).x
	rpcgen $(APPN).x
	awk 'BEGIN {print "extern int uid,gid,gidlen,gidlist[16];\n"} { print $$0; if(($$1=="case")&&($$2=="DELETE:")) printf("AUTH$(AUTH)(rqstp,transp,uid,gid,gidlen,gidlist)\n") }' \
	< $(APPN)_svc.c | into $(APPN)_svc.c

