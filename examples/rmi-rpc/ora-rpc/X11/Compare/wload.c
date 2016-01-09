#include <stdio.h>
#include <pixrect/pixrect_hs.h>
#include <suntool/sunview.h>
#include <suntool/canvas.h>
#define error(a) {fprintf(stderr, "Usage: %s rasterfile\n", a); exit(-1);}

void            MouseCaught();
Pixwin         *pw;
colormap_t      cm;
struct rasterfile rast;
FILE           *fp;
Pixrect        *pr_src;
u_char         *d;


main(argc, argv)
  int             argc;
  char          **argv;
{
  Frame           frame;
  Canvas          canvas;
  register int    i;

  /* read the source rasterfile */
  if (argc != 2)
    error(argv[0]);
  fp = fopen(argv[1], "r");
  if (!fp || !(d = (u_char *) ReadSunRaster(fp, &cm, &rast)))
    perror("can't read rasterfile\n");

  /* create frame and canvas */
  frame = window_create(NULL, FRAME,
			FRAME_LABEL, argv[1],
			0);
  canvas = window_create(frame, CANVAS,
			 WIN_HEIGHT, rast.ras_height,
			 WIN_WIDTH, rast.ras_width,
			 WIN_EVENT_PROC, MouseCaught,	/* kill window */
			 0);
  window_fit(frame);

  /* get the canvas pixwin to draw into */
  pw = canvas_pixwin(canvas);


  /* build and install a ramp color table if the image doesn't have one */
  if (cm.type == RMT_NONE) {
    unsigned char   grey[256];
    for (i = 0; i < 256; i++)
      grey[i] = (unsigned char) i;
    pw_setcmsname(pw, "original");
    pw_putcolormap(pw, 0, 256, grey, grey, grey);
  } else {
    pw_setcmsname(pw, "original");
    pw_putcolormap(pw, 0, cm.length,
		   cm.map[0], cm.map[1], cm.map[2]);
  }

  /* load the image to the pw manually */
  pr_src = mem_point(rast.ras_width, rast.ras_height, rast.ras_depth,
		     (short *) d);
  pw_write(pw, 0, 0, rast.ras_width, rast.ras_height, PIX_SRC, pr_src, 0, 0);
  pr_destroy(pr_src);

  /* loop till dead */
  window_main_loop(frame);
}

void
MouseCaught(canvas, event, arg)
  Canvas          canvas;
  Event          *event;
  caddr_t         arg;
{
  if (event_action(event) == MS_RIGHT) {
    exit(0);
  }
}
