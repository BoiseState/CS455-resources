APPN     = rtrace
include ../makefile.template

INC = -I../X11/Compare raster.c # a couple kludges to re-use raster stuff

# note - do't try to use -O4 with signals... use a *XView=[01] flag here*
XView    = 0
CFLAGS   = $(INC) -O2 -I${OPENWINHOME}/include  -DXView=${XView}
LFLAGS   = -lm -L${OPENWINHOME}/lib -lxview -lolgx -lX11
