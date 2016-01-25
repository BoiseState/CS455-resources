INCLUDE         = -I${OPENWINHOME}/include

# if you want special to pass special loader options to ld, set
# LDFLAGS= ...
#

X11_LIBS        = -lX11 
XVIEW_LIBS      = -L${OPENWINHOME}/lib -lxview -lolgx
SV_LIBS      	 = -lpixrect -lsuntool -lsunwindow

#
# If you want to compile for debugging, change "-O" to "-g"
#

CFLAGS          = ${INCLUDE} -g
LFLAGS          = 

.c.o:
	$(CC) -o $*.o -c $(CFLAGS) $< $(LFLAGS)

ALL = wload xwload

xwload: xwload.c raster.o
	$(CC) -o xwload $(CFLAGS) xwload.c raster.o $(LFLAGS) ${X11_LIBS} ${XVIEW_LIBS}

wload: wload.c raster.o
	$(CC) -o wload $(CFLAGS) wload.c raster.o $(LFLAGS) ${SV_LIBS}

clean:
	$(RM) $(ALL) *.o
