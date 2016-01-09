APPN = rls
include ../../makefile.template

RSRC=$(APPN).x $(APPN)_svc_proc.c $(APPN).c
CFLAGS= -g 	# turn on debugging,  cc builds tables

# Build a local-running, dbxtool-able version w/ no XDR or RPC use.  Note for
# link to succeed, keep your xdr_free()'s and the like in one compilation
# unit (file).  I keep them in $(APPN)_svc_proc.c.

local: $(APPN).h $(APPN).c $(APPN)_svc_proc.c
	egrep -v xdr_ $(APPN)_svc_proc.c>`date|awk '{print $$1$$2$$3}'`_svc_proc.c
	egrep -v clnt_create $(APPN).c>`date|awk '{print $$1$$2$$3}'`.c
	$(CC) -g -DDEBUG -o local `date|awk '{print $$1$$2$$3}'`.c \
		`date|awk '{print $$1$$2$$3}'`_svc_proc.c $(LFLAGS)
	# $(RM) `date|awk '{print $$1$$2$$3}'`.c and 
	#	`date|awk '{print $$1$$2$$3}'`_svc_proc.c after dbx-ing.
	# Do not edit them, change $(APPN)_svc_proc.c and $(APPN).c.

# OR if you have the #defines mentioned in $(APPN).c to remove
# clnt_create(), and that's your only use of RPC, try this.  Note use of
# -DDEBUG conditional compile flag, assumes
localD: $(APPN).h $(APPN).c $(APPN)_svc_proc.c
	egrep -v xdr_ $(APPN)_svc_proc.c>`date|awk '{print $$1$$2$$3}'`_svc_proc.c
	$(CC) -g -DDEBUG -o localD  $(APPN).c \
		`date|awk '{print $$1$$2$$3}'`_svc_proc.c $(LFLAGS)
	# $(RM) `date|awk '{print $$1$$2$$3}'`_svc_proc.c after dbx-ing.
	# Do not edit it, change $(APPN)_svc_proc.c.
