INCLUDE         = -I${OPENWINHOME}/include -I../../../rls/manual/minimum

#
# If you want to compile for debugging, change "-O" to "-g"
#

CFLAGS          = ${INCLUDE} -g

# if you want special to pass special loader options to ld, set
# LDFLAGS= ...
#

X11_LIBS        = -lX11 
XVIEW_LIBS      = -L${OPENWINHOME}/lib -lxview -lolgx
XT_LIBS      	 = -L${OPENWINHOME}/lib -lXt -lXol

ALL             = udpDispatchServer tcpDispatchServer
OBJ             = rls_xdr.o read_dir.o dispatch.o

all: $(ALL)

# the following is compatible with the ../../../rls/manual/minimum/rls client
udpDispatchServer: udpDispatchServer.c $(OBJ)
	$(CC) -o udpDispatchServer $(CFLAGS) udpDispatchServer.c $(OBJ) $(LFLAGS) ${XT_LIBS} ${X11_LIBS}

# the following is compatible with the ../../../rls/manual/minimum/tcpRls client
tcpDispatchServer: tcpDispatchServer.c $(OBJ)
	$(CC) -o tcpDispatchServer $(CFLAGS) tcpDispatchServer.c $(OBJ) $(LFLAGS) ${XT_LIBS} ${X11_LIBS}

clean:
	$(RM) *.o $(ALL)
