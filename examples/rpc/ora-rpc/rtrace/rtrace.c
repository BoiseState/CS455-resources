#include <stdio.h>
#include <netdb.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <rpc/rpc.h>
#if XView == 1
#include <X11/X.h>
#include <X11/Xlib.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/notify.h>
#include <xview/cms.h>
#endif XView
#include "rtrace.h"
#include "rtrace_shared.h"

/*
 * Global server/client relationship stuff - max servers <=
 * 32.
 */
int             NumServers;	/* total # of servers
				 * connected */
int             ServersFree = 0x0;	/* bits are 1 if free, 0
					 * if busy */
int             ServerPid[MAXSERV];	/* used to record
					 * fork()'ed pids */
char            ServerName[MAXSERV][32];	/* record server names */
int             ServerLoad[MAXSERV];	/* server load
					 * capability */
int             ServerTot[MAXSERV];	/* # times the server
					 * was used */
long            ServerTime[MAXSERV];
long            ServerStart[MAXSERV];
CLIENT         *cl[MAXSERV];	/* client handles */
struct timeval  DTime = {480, 0};
int             MaxServ = 8;
int             InitServ = TRUE;
int             LoadSizing = FALSE;	/* don't do dynamic load
					 * balancing */
/*
 * Global graphic, display and I/O stuff.
 */
double          bkcon = 1.0;	/* background contrast */
char           *spat;		/* background file name */
balls           bl;		/* ball vector */
int             height, width;
ray            *pRays;
reply          *pReply;
FILE           *pFBal,		/* ball input data file */
               *pFpat,		/* bkgnd. pattern input
				 * data file */
#if XView == 0
               *pFOut,		/* output data file */
#endif XView
               *pFRhosts = NULL;/* rhosts file */

#if XView == 1
/*
 * X11 and xview stuff - yuck blech rech.  I tend to keep
 * them global as I never know when I might need them.  I
 * do occasionally step on them (e.g. in the repaint
 * procedure).
 */
Frame           frame;
Canvas          canvas;
Xv_cmsdata      cms_data;
Cms             cms;
int             scrn;
XImage         *ximage;
GC              gc;
XGCValues       gcvalues;
unsigned long   gcmask;
Display        *dpy;

/*
 * My own vestigal rasterfile pixel I/O stuff
 */
colormap_t      cm;
rasterfile      rast;
#endif XView

/*
 * Signaling, timing and error handling.
 */
extern int      errno;
struct timeval  tval;
#if XView == 0
void            CatchServer();
#endif XView
#if XView == 1
int             CatchServer();
#endif XView

char           *
NFSgetcwd()
{
  char            path_name[MAXPATHLEN], *getcwd();

  getcwd(path_name, MAXPATHLEN);
  /* remove any local /tmp_mnt/home NFS mount problems */
  while (path_name[1] != 'h')
    strcpy(&(path_name[1]), &(path_name[2]));
  return (path_name);
}

main(argc, argv)
  int             argc;
  char          **argv;
{
  ray             rr;
  vector          vp;
  double          x, y, z;
  sphere          ls;
  parameters      params;
  int             i;
  char           *d;
#if XView == 0
  rasterfile      RasOut;
#endif XView

  /* Parse command line options */
  CommandLine(argc, argv);

  /* Read ~/.rhosts and attempt to start/contact servers */
  StartServers(argv, 0);

#if XView == 1
  /* Open the window */
  width = (int) ((XMAX - XMIN) * SCALE + 0.9999999);
  height = (int) ((YMAX - YMIN) * SCALE + 0.9999999);
  INITRAST(rast, width, height);
  WindowInit(argc, argv);
#endif XView

  /* Make any new child deaths be recorded by CatchServer() */
  while (wait((union status *) NULL) != -1) {
    sleep(STIME);
  }
  signal(SIGCHLD, CatchServer);

  /* Bring in ball data and ship it to the servers. */
  if (InitServ == TRUE) {
    GetBalls();
    EachServerOnce(load_balls_1, &bl);
    /*
     * Send NFS qualified filename for background to
     * servers to read. The easiest way to get at a RLL filter 
     * under SunOS is in /usr/lib/rasfilters.  That really speeds
     * things up.
     */
    EachServerOnce(load_bkgnd_1, &spat);
  }
  /*
   * Establish viewpoint & light source vector, background
   * contrast
   */
  MV(95.0, 140.0, -200.0, vp);
  MV(0.0, 900.0, 0.0, ls.cent);
  ls.rad = 40;

  /*
   * Servers need to know light source & viewpoint vector,
   * background contrast
   */
  params.ls = &ls;
  params.vp = &vp;
  params.bkcon = &bkcon;
  EachServerOnce(set_params_1, &params);

#if XView == 0
  /* Write a header and blank raster data to disk */
  width = (int) ((XMAX - XMIN) * SCALE + 0.9999999);
  height = (int) ((YMAX - YMIN) * SCALE + 0.9999999);
  INITRAST(RasOut, width, height);
  WriteSunRaster(pFOut, NULL, &RasOut, NULL);

  /* Cast and trace all the rays at servers */
  if (NumServers > 1)
    LoadSizing = TRUE;		/* Turn-on load sizing */

  /* Make sure all the children are back */
  while (wait((union status *) NULL) != -1) {
    sleep(STIME);
  }

  ReqRay();
  for (i = 0; i < NumServers; i++)
    printf("%s: %d\n", ServerName[i], ServerTot[i]);
#else XView
  /*
   * Good nite!  All the action starts when a CANVAS_REPAINT
   * event occurs.
   */
  xv_main_loop(frame);
#endif XView
}

CommandLine(argc, argv)
  int             argc;
  char          **argv;
{
  int             i, c, in = 0, out = 0, tex = 0;
  char            resolved_name[MAXPATHLEN];

#if XView == 1
  /*
   * Take a look at and modify the command line if
   * necessary, then read-in input image to size things-up
   * - could use an XImage, but that`s not necessary if the
   * data stays here....
   */
  xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);
#endif XView

  for (i = 1; i < argc; i++) {
    if (argv[i][0] != '-')
      ERRMSG("Start with a '-' ! ");
    c = argv[i][1];

    switch (c) {
    case ('i'):
      if (in)
	ERRMSG("You may only have one input file");
      in = 1;
      if ((i + 1) >= argc || argv[i + 1][0] == '-')	/* no arg */
	pFBal = stdin;
      else if ((pFBal = fopen(argv[++i], "r")) == NULL)
	ERRMSG("Input file not found");
      break;
#if XView == 0
    case ('o'):
      if (out)
	ERRMSG("You may have only one output file");
      out = 1;
      if ((i + 1) >= argc || argv[i + 1][0] == '-')	/* no arg */
	pFOut = stdout;
      else
	pFOut = fopen(argv[++i], "w");
      break;
#endif XView
    case ('b'):
      if (tex)
	ERRMSG("You may have only one image file");
      if ((i + 1) >= argc || argv[i + 1][0] == '-')	/* no arg */
	ERRMSG("-b requires an argument");
      tex = 1;
      realpath(argv[++i], resolved_name);
      if ((pFpat = fopen(resolved_name, "r")) == NULL)
	ERRMSG("Background rasterfile not found");
      fclose(pFpat);
      spat = argv[i];
      break;
    case ('n'):
      if (argv[i][2] < '0' || argv[i][2] > '9') {
	printf("%c\n", argv[i][2]);
	ERRMSG("-n needs a numerical argument");
      }
      MaxServ = atoi(&(argv[i][2]));
      break;
    case ('B'):
      if (argv[i][2] < '0' || argv[i][2] > '9') {
	printf("%c\n", argv[i][2]);
	ERRMSG("-B needs a numerical argument");
      }
      bkcon = atof(&(argv[i][2]));
      break;
    case ('s'):
      if ((i + 1) >= argc || (!isdigit(argv[i + 1][0])))
	/* no good arg */
	ERRMSG("-s requires a numeric argument");
      if (1 != sscanf(argv[++i], "%d", &MaxServ))
	ERRMSG("-s requires a numeric argument");
      if (MaxServ >= sizeof(int) * 8)
	ERRMSG("# servers must be < sizeof(int)*8");
      break;
    case ('d'):
      if ((i + 1) < argc && argv[i + 1][0] != '-')
	ERRMSG("-d doesn't take a parameter");
      InitServ = FALSE;
      ++i;
      break;
    default:
      ERRMSG("Unrecognized option. Better try again");
    }
  }

  if (!in)
    if ((pFBal = fopen("bdata.i", "r")) == NULL)
      ERRMSG("bdata.i not found");
#if XView == 0
  if (!out)
    pFOut = fopen("data.dis", "w");
#endif XView
  if (!tex) {
    sprintf(resolved_name, "%s/pat.def", NFSgetcwd());
    if ((pFpat = fopen(resolved_name, "r")) == NULL) {
      WARNMSG(resolved_name);
      ERRMSG("not found");
    }
    fclose(pFpat);
    spat = strdup(resolved_name);
  }
}

StartServers(argv, online)	/* recursive */
  char           *argv[];
  int             online;
{
  char            sBuf[128];
  int             i, j;
  long            ts;

  if (!pFRhosts) {		/* is it open already? */
    sprintf(sBuf, "%s%s", getenv("HOME"), "/.rhosts");
    if (!(pFRhosts = fopen(sBuf, "r")))
      ERRMSG("Can't open ~/.rhosts to get server names");
  }
  for (i = online; (i < MaxServ) &&
       (fscanf(pFRhosts, "%s%s", ServerName[i], sBuf) == 2); i++) {

    cl[i] = clnt_create(ServerName[i], RTRACESERVER, RTRACEVERS, "tcp");

    /*
     * If attempt to contact fails, try to (re)start the
     * server.
     */
    if (!cl[i]) {
      struct servent *out;
      char c;
      char *h;
      int rem;
      h = ServerName[i];

      out = getservbyname("exec", "tcp");
      sprintf(sBuf, "%s/%s_svc < /dev/null >& /tmp/%s &",
	    NFSgetcwd(), argv[0], argv[0]);
      if (!fork()) {
       fprintf(stdout, "trying to start %s\n", ServerName[i]);
       rem = rexec(&h, out->s_port, NULL, NULL, sBuf, 0);
       while(read(rem, &c, 1)) fprintf(stderr, "%c", c);
	_exit(0);
      }				/* pipeline the rexec's! */
    }
  }
  NumServers = i;

  /*
   * Keep checking to see if they're all alive. If it doesn't
   * happen within REXETIME seconds, move on...
   */
  for (i = j = online; i < NumServers; i++) {
    gettimeofday(&tval, NULL);
    ts = tval.tv_sec;

    while (tval.tv_sec - ts <= REXETIME) {	/* wait for a while */
      cl[j] = clnt_create(ServerName[i], RTRACESERVER, RTRACEVERS, "tcp");
      if (cl[j])
	break;
      /* It failed, try again. */
      sleep(STIME);
      gettimeofday(&tval, NULL);
    }

    if (cl[j]) {
      /* Designate an available server */
      printf("server running on %s\n", ServerName[j]);
      ServersFree |= (0x1 << j);
      strcpy(ServerName[j], ServerName[i]);
      ServerPid[i] = 0;
      ServerLoad[j] = DEFLOAD;
      ServerTot[j] = 0;
      ServerTime[j] = 1000;

      /*
       * To avoid having to use svc_sendreply() in servers,
       * delay time-outs and use forks locally
       */
      clnt_control(cl[j], CLSET_TIMEOUT, (char *) &DTime);
      j++;
    } else {			/* it failed even after
				 * REXETIME seconds! */
      clnt_pcreateerror(ServerName[i]);
    }
  }
  NumServers = j;

  /* Repeat until EOF or MaxServers really available */
  if ((!feof(pFRhosts)) && (NumServers < MaxServ))
    StartServers(argv, NumServers);

  if (pFRhosts)
    fclose(pFRhosts);
}

GetBalls()
{
  int             i;
  double          x, y, z, r, ior, rfr, rfl, dif, amb;
  ball           *bp;

  bl.balls_val = (ball *) malloc(sizeof(ball) * MAXBALLS);

  for (i = 0;
       fscanf(pFBal, "%lf %lf %lf %lf %lf %lf %lf %lf %lf",
     &x, &y, &z, &r, &ior, &rfr, &rfl, &dif, &amb) != EOF;
       i++) {
    if (i >= MAXBALLS)
      ERRMSG("Too many balls, MAXBALLS max.");
    bp = bl.balls_val + i;
    bp->s.cent.x = x;
    bp->s.cent.y = y;
    bp->s.cent.z = z;
    bp->s.rad = r;
    bp->ior = ior;
    bp->rfr = rfr;
    bp->rfl = rfl;
    bp->dif = dif;
    bp->amb = amb;
  }
  bl.balls_len = i;
}

#if XView == 0
void 
#else XView
/*
 * Use a SIGCHLD handler to catch returning children.
 */
#endif XView
CatchServer()
{
  int             Server;
  int             DeadPid;
  union wait      status;

  DeadPid = wait(&status);
  if (DeadPid == -1)
    return;			/* doesn't happen if it's
				 * installed as a signal
				 * handler for SIGCHLD */

  /*
   * Catch them as they return, inspecting their exit()
   * status
   */
  for (Server = 0; Server < NumServers; Server++) {
    if (ServerPid[Server] == DeadPid) {
      /* if server failed, replace it */
#ifdef DBXTOOL
      if (status.w_status != 133) {	/* use this if window
					 * debug */
#else
      if (status.w_T.w_Retcode != SUCCESS) {
#endif
	printf("server %s had %d trouble and is being disabled\n",
	       ServerName[Server], status.w_T.w_Retcode);
	ServerPid[Server] = 0;	/* Mark it as returned */
      } else {
	if (LoadSizing) {
	  gettimeofday(&tval, NULL);
	  ServerTime[Server] = tval.tv_sec - ServerStart[Server];
	  if (Total(ServerTime, NumServers) > (NumServers << 2)) {
	    /* slow server down with more rays, low pass */
	    if (ServerTime[Server] * NumServers <
		Total(ServerTime, NumServers)) {
	      ServerLoad[Server] = MIN(ServerLoad[Server] + 1, MAXLOAD);
	    } else {		/* speed it up */
	      ServerLoad[Server] = MAX(ServerLoad[Server] - 1, MINLOAD);
	    }
	  }
	  printf("%s took %d secs., load now %d\n",
		 ServerName[Server], ServerTime[Server], ServerLoad[Server]);
	}
	ServerTot[Server]++;
	ServerPid[Server] = 0;	/* Mark it as returned */
	ServersFree |= (0x1 << Server);	/* Mark it as available */
      }
      return;
    }
  }
  /* the returned child is not registred */
  printf("panic: a bastard child!");
}

#define myexit(val) _exit(val);
#ifdef DEBUG			/* handy */
#define myexit(val) {printf("child %d back from %s exit w/ %d\n",getpid(),ServerName[Server],val);_exit(val);}
#endif

EachServerOnce(proc, arg)	/* pipeline broadcast
				 * mechanism */
  char           *(*proc) ();
  char           *arg;
{
  int             Server;
  int             pid;		/* Need this as CatchServer
				 * could step on
				 * ServerPid[Server] */
  int             SMask;

  /*
   * Request a remote call of each server - only if healthy
   * (marked available).  If call fails, ServerPid is
   * cleared by CatchServer, ServersFree is left set.
   */
  for (Server = 0; Server < NumServers; Server++)
    CatchServer();
  if (ServersFree == 0x0)
    ERRMSG("No healthy servers left!");
  for (Server = 0; Server < NumServers; Server++) {
    SMask = 0x1 << Server;
    /* Check to see if it's sick */
    if (!(SMask & ServersFree))
      continue;
    /*
     * Wait for it to come back if necessary - order could
     * be important
     */
    if (ServerPid[Server]) {
      printf("Waiting for initialization processes to return\n");
      sleep(STIME);
    }
    ServersFree ^= SMask;	/* Mark it as unavailable -
				 * and let's go */
    while ((pid = ServerPid[Server] = fork()) == -1) {
      perror();
      sleep(STIME);		/* fork failed, sleep a
				 * while */
    }
    if (pid)
      continue;

    /* child */
    pReply = (reply *) (*proc) (arg, cl[Server]);
    if ((pReply == NULL) || (pReply->op == ERROR)) {
      clnt_perror(cl[Server], ServerName[Server]);
      fprintf(stderr, "%s", pReply->reply_u.sOp);
      myexit(ERROR);
    }
    myexit(SUCCESS);		/* success, child dies
				 * without consequences */
  }
}				/* parent leaving with
				 * living children... */

Total(vect, max)
  int             vect[MAXSERV], max;
{
  int             i, total = 0;
  for (i = 0; i < max; i++)
    total += vect[i];
  return (total);
}

#if XView == 0
ReqRay()
#else XView
/*
 * We'll use some Xlib stuff to get the pixels (an XImage)
 * out in this repaint callback procedure. It launches
 * children to repaint the window, just to show the use of
 * children async. processing and reaping.
 */
ReqRay(canvas, paint_window, dpy, win)
  Canvas          canvas;
  Xv_Window       paint_window;	/* not used */
  Display        *dpy;
  Window          win;
#endif XView
{
  int             i;
  double          yco;

#if XView == 1
  /* Cast and trace all the rays at servers */
  if (NumServers > 1)
    LoadSizing = TRUE;		/* Turn-on load sizing */

  /*
   * Note a maximum-sized dummy XImage was already made in
   * the WindowInit() call - gives child processes dummies
   * - saves LOTS of time
   */
#endif XView

  /* Start casting rays... */
  for (yco = YMAX * SCALE; yco > YMIN * SCALE;) {
    if (InitServ)
      printf("Scan-line %d\n", (int) yco);

    /* launch the next free server, w/ load sizing */
#if XView == 0
    ForkNextServer(&yco);
#else XView
    ForkNextServer(&yco, win);
#endif XView
  }

#if XView == 1
  for (i = 0; i < NumServers; i++)
    printf("%s: %d\n", ServerName[i], ServerTot[i]);
#endif XView
}

/*
 * ForkNextServer() - Fork and start remote procedure on
 * the server (let Unix manage resource contention and
 * detailed scheduling!). Record allocation and pid in
 * ServersFree and ServerPid[], resp.
 */
#if XView == 0
ForkNextServer(pyco)
#else XView
ForkNextServer(pyco, win)
  Window          win;
#endif XView
  double         *pyco;
{
  lines           lns;
  int             Server;	/* Number of who gets
				 * allocated */
  int             SMask;
  replypix       *prp;
  int             Ppid;
  int             height;

  /*
   * Direct computations to a free server.  The servers
   * could get all allocated so might have to wait for the
   * next available server.
   */
  Ppid = getpid();
  while (ServersFree == 0x0) {
    CatchServer();
    sleep(STIME);
  }
  for (Server = 0; Server < NumServers; Server++) {
    /* Allocate the next free server */
    SMask = 0x1 << Server;
    if (SMask & ServersFree) {
      ServersFree ^= SMask;

      /*
       * Watch the timer, establish loading proportional to
       * last speed of response
       */
      lns.ymax = *pyco;
      lns.ymin = (*pyco -= ServerLoad[Server]);
      gettimeofday(&tval, NULL);
      ServerStart[Server] = tval.tv_sec;

      while ((ServerPid[Server] = fork()) == -1) {
	perror("can't fork another server (sleeping)");
	if (EAGAIN == errno) {
	  printf("ran out of processes (sleeping)");
	  errno = 0;
	}
	CatchServer();
	sleep(STIME);		/* fork failed, sleep to
				 * catch return child */
      }

      if (getpid() == Ppid)
	return (SUCCESS);
      break;
    }
  }

  /* Children only */
  prp = ray_trace_1(&lns, cl[Server]);
  height = (int) (lns.ymax - lns.ymin);
  if ((prp == NULL) ||
      (prp->replypix_len != height * width)) {
    clnt_perror(cl[Server], ServerName[Server]);
    _exit(ERROR);
  }
#if XView == 0
  /* Seek to the right position and write it out */
  fflush(pFOut);
  fseek(pFOut, (long) (width * (YMIN * SCALE - lns.ymax)), 2);
  if (prp->replypix_len == fwrite(prp->replypix_val,
		sizeof(char), prp->replypix_len, pFOut)) {
    fflush(pFOut);
#else XView
  /*
   * Stuff the pixels in the window and exit - must pass
   * thru both an XImage
   */
  ximage->data = prp->replypix_val;
  XPutImage(dpy, win, gc, ximage, 0, 0, 0, (int) (YMAX * SCALE - lns.ymax),
	    width, height);
  fflush(stdout);
  XFlush(dpy);
#endif XView
    _exit(SUCCESS);
  }
#if XView == 0
  _exit(ERROR);
}

#else XView

WindowInit(argc, argv)
  int             argc;
  char           *argv[];
{
  int             x;

  /*
   * build and install a ramp color table if the image
   * doesn't have one
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
			    FRAME_LABEL, argv[0],
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
			      CANVAS_REPAINT_PROC, ReqRay,
			      OPENWIN_AUTO_CLEAR, FALSE,
			      WIN_DYNAMIC_VISUAL, TRUE,
			      WIN_CMS_NAME, "XisBrainDead",
			      WIN_CMS_DATA, &cms_data,
			      NULL);
  window_fit(frame);

  /*
   * The following section initializes some structures for
   * repeated use in child processes - save the trouble of
   * re-creating them each time.  It has the side effects
   * of initializing some global X11/Xview variables.
   * 
   * First create an XImage...
   */
  dpy = (Display *) xv_get(canvas, XV_DISPLAY);
  scrn = DefaultScreen(dpy);
  ximage = XCreateImage(dpy, DefaultVisual(dpy, scrn),
       rast.ras_depth, ZPixmap, 0, (unsigned char *) NULL,
			width, MAXLOAD, 8, width);

  /*
   * ... then create a GC ...
   */
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
}
#endif XView
