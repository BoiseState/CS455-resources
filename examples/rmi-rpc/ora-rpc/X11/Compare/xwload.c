/*
 * A sample X11/XView program that stuffs Sun rasterfile format images into a
 * window.  It uses XImages and Pixmaps.
 */
#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/cms.h>
#include "raster.h"
#define error(a) {fprintf(stderr, "Usage: %s rasterfile\n", a); exit(-1);}

/*
 * X11 and xview stuff
 */
Frame           frame;
Canvas          canvas;
Xv_cmsdata      cms_data;
Cms             cms;
int             scrn;
XImage         *ximage;
Pixmap          pixmap;
GC              gc;
XGCValues       gcvalues;
unsigned long   gcmask;
Display        *dpy;


/*
 * My own pixel I/O stuff
 */
u_char         *d;
colormap_t      cm;
rasterfile      rast;
FILE           *fp;

/*
 * We'll use some Xlib stuff to get the pixels (a Pixmap) out
 */
void
repaint_proc(canvas, paint_window, dpy, win)
  Canvas          canvas;
  Xv_Window       paint_window;	/* not used */
  Display        *dpy;
  Window          win;
{
  XCopyArea(dpy, pixmap, win, gc, 0, 0, rast.ras_width, rast.ras_height, 0, 0);
}

main(argc, argv)
  int             argc;
  char           *argv[];
{
  int             x, y;
  /*
   * Take a look at and modify the command line if necessary, then read-in
   * input image to size things-up - could use an XImage, but that`s not
   * necessary if the data stays here....
   */
  xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);
  if (argc != 2)
    error(argv[0]);
  fp = fopen(argv[1], "r");
  if (!fp || !(d = (u_char *) ReadSunRaster(fp, &cm, &rast)))
    perror("can't read rasterfile\n");

  /*
   * build and install a ramp color table if the image doesn't have one
   */
  cms_data.type = XV_DYNAMIC_CMS;
  cms_data.index = 0;

  if (cm.type == RMT_NONE) {
    unsigned char   grey[256];
    cms_data.size = cms_data.rgb_count = 256;
    for (x = 0; x < 256; x++)
      grey[x] = (unsigned char) x;
    cms_data.red = cms_data.green = cms_data.blue = grey;
  } else {
    cms_data.size = cms_data.rgb_count = cm.length;
    cms_data.red = cm.map[0];
    cms_data.green = cm.map[1];
    cms_data.blue = cm.map[2];
  }


  /*
   * Establish a window w/ a repaint procedure 
   */
  frame = (Frame) xv_create(NULL, FRAME,
			    FRAME_LABEL, argv[1],
			    FRAME_SHOW_FOOTER, TRUE,
			    NULL);
  canvas = (Canvas) xv_create(frame, CANVAS,
			      WIN_WIDTH, rast.ras_width,
			      WIN_HEIGHT, rast.ras_height,
			      CANVAS_WIDTH, rast.ras_width,
			      CANVAS_HEIGHT, rast.ras_height,
  /* don't retain window -- we'll repaint it all the time */
			      CANVAS_RETAINED, FALSE,
  /* We're using Xlib graphics calls in repaint_proc() */
			      CANVAS_X_PAINT_WINDOW, TRUE,
			      CANVAS_REPAINT_PROC, repaint_proc,
			      OPENWIN_AUTO_CLEAR, FALSE,
			      WIN_DYNAMIC_VISUAL, TRUE,
			      WIN_CMS_NAME, "XisBrainDead",
			      WIN_CMS_DATA, &cms_data,
			      NULL);
  window_fit(frame);

  /*
   * All I want to do is stream some pixels into a window.... this kind of
   * thing was easy w/ low-level windowing systems. I guess to get at this
   * huge new audience, I`ll jump thru hops...
   * 
   * I'll create an XImage out of the raw pixel data.  I really want a Pixmap,
   * as I know I'm at the display server, or I wouldn't want to look at this
   * image...  XImages have LOTS of extra baggage, as they are designed for
   * X11 client use - then passed to server w/ XPutImage().  Pixmaps live at
   * the display server, and have little or now penalty associated w/ them.
   * XImages and RPCs don't mix - XImages should USE RPCs or at least XDR. If
   * X11 had a better suite of display server Pixmap I/O routines, or would
   * allow me to find a real RAM address for the pixels, I could have avoided
   * all this!!!!
   * 
   * Interesting note - my comparison of old sunview() pw_* operations and the
   * Xlib drawing stuff, as implemented on a Sun, is almost a 2X performance
   * hit!
   * 
   * First create an XImage...
   */
  dpy = (Display *) xv_get(canvas, XV_DISPLAY);
  scrn = DefaultScreen(dpy);
  ximage = XCreateImage(dpy, DefaultVisual(dpy, scrn),
			rast.ras_depth, ZPixmap, 0, d,
			rast.ras_width, rast.ras_height, 8, rast.ras_width);

  /*
   * ... then create a pixmap and the associated GC ...
   */
  pixmap = XCreatePixmap(dpy, DefaultRootWindow(dpy),
			 rast.ras_width, rast.ras_height, rast.ras_depth);

  gcvalues.function = GXcopy;
  gcmask |= GCFunction;
  gcvalues.plane_mask = AllPlanes;
  gcmask |= GCPlaneMask;
  gcvalues.foreground = 1;
  gcmask |= GCForeground;
  gcvalues.background = 0;
  gcmask |= GCBackground;
  gcvalues.graphics_exposures = False;
  gcvalues.background = WhitePixel(dpy, DefaultScreen(dpy));
  gcvalues.foreground = BlackPixel(dpy, DefaultScreen(dpy));
  gc = XCreateGC(dpy, DefaultRootWindow(dpy), gcmask, &gcvalues);

  /*
   * ... now get the image over here!  This is slow - keep it local!
   */
  XPutImage(dpy, pixmap, gc, ximage, 0, 0, 0, 0,
	    rast.ras_width, rast.ras_height);
  XDestroyImage(ximage);

  /*
   * Good nite!
   */
  xv_main_loop(frame);
}
