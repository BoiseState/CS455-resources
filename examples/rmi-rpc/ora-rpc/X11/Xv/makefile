INCLUDE         = -I${OPENWINHOME}/include

# if you want special to pass special loader options to ld, set
# LDFLAGS= ...
#

X11_LIBS        = -lX11 
XVIEW_LIBS      = -L${OPENWINHOME}/lib -lxview -lolgx
XT_LIBS      	= -L${OPENWINHOME}/lib -lXt -lXol

#
# If you want to compile for debugging, change "-O" to "-g"
#

CFLAGS          = ${INCLUDE} -g
LFLAGS          = 

.c :
	$(CC) -o $* $(CFLAGS) $< rls_xdr.o $(LFLAGS) ${X11_LIBS} ${XVIEW_LIBS}

# 	The itimer and syncCallback clients are compatible with sync. UDP rls server 
# (e.g. ./notifyServer).
# 	The asyncCallback client is compatible with an async FRPC server 
# like ../../rls/manual/async/asyncRls_svc.

ALL = notifyServer tcpNotifyServer asyncCallback syncCallback itimer

all: $(ALL)

notifyServer: notifyServer.c rls_xdr.o dispatch.o read_dir.o
	${CC} -o notifyServer notifyServer.c dispatch.o rls_xdr.o \
	  read_dir.o $(CFLAGS) ${XVIEW_LIBS} ${X11_LIBS}

tcpNotifyServer: tcpNotifyServer.c rls_xdr.o dispatch.o read_dir.o
	${CC} -o tcpNotifyServer tcpNotifyServer.c dispatch.o rls_xdr.o \
	  read_dir.o $(CFLAGS) ${XVIEW_LIBS} ${X11_LIBS}

clean:
	$(RM) $(ALL) *.o
