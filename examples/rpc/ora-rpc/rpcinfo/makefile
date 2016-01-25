CFLAGS=	-O
LDFLAGS= $(LIB)

ALL = rpcinfo	# note the patchFile has been applied to produce this
					# version of rpcinfo
all:	${ALL}

rpcinfo:	rpcinfo.c getopt.c
	${CC} ${CFLAGS} -o $@ $@.c getopt.c ${LDFLAGS}
clean:
	$(RM) $(ALL) *.o
