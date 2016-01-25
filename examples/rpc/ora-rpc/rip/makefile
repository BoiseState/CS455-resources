APPN     = rip
include ../makefile.template

CFLAGS   = #-O2  # note - do't try to use -O4 with signals...
LFLAGS   = -lm -lpixrect -lsuntool -lsunwindow

#got a FPU?: CFLAGS   = -f68881, LFLAGS   = /usr/lib/f68881/libm.il

# run the rip application like this:
#	rip rip_script 512 512
# you must select the opened window to process each line of the script
