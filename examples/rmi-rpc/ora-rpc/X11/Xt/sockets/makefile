INCLUDE         = -I${OPENWINHOME}/include

#
# If you want to compile for debugging, change "-O" to "-g"
#

CFLAGS          = ${INCLUDE} -g

# if you want special to pass special loader options to ld, set
# LDFLAGS= ...
#

X11_LIBS        = -lX11 
XVIEW_LIBS      = -L${OPENWINHOME}/lib -lxview -lolgx
XT_LIBS      	= -L${OPENWINHOME}/lib -lXol -lXt

.c :
	$(CC) -o $* $(CFLAGS) $< $(LFLAGS) ${XT_LIBS} ${X11_LIBS}

# run against multiple socket IPC servers 
# like ../../../ipc/sockets/Loop_TCP_AF_INETserver;
Xt_sockets_AF_INET_client:

clean:
	$(RM) Xt_sockets_AF_INET_client
