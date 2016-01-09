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
XT_LIBS      	= -L${OPENWINHOME}/lib -lXt -lXol

hello: hello.c
	$(CC) -o hello $(CFLAGS) hello.c $(LFLAGS) ${XT_LIBS} ${X11_LIBS}

clean:
	$(RM) *.o hello
