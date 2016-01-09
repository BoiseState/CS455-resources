/*
 * rip.c: remote image processing client main.
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <suntool/sunview.h>
#include <suntool/canvas.h>
#include <rpc/rpc.h>
#include "rip.h"
#include "rip_shared.h"

/*
 * Global server/client relationship stuff - max servers <= 8.
 */
char            ServersFree = 0x0;	/* bits are 1 if free, 0 if busy */
int             NumServers;		/* total # of servers connected */
int             ServerPid[MAXSERVERS];	/* used to record fork()'ed pids */
char            ServerName[MAXSERVERS][32];	/* record server names */
CLIENT         *cl[MAXSERVERS];			/* client handles */

/*
 * Client <--> server data structures, defined in protocol.
 */
Packet         *pRequest;
Packet         *pReply;
struct timeval  DebugTime;	/* fool w/ time out during REMOTE_debug */

/*
 * Global graphic, display and I/O stuff.
 */
Kernel         *pKernel = NULL;	/* list of active kernels */
Image          *pImage = NULL;	/* list of active images */
Pixwin         *pPw;		/* the window canvas for the data */
Pixrect        *pPr;		/* the pixrect behind the window */
colormap_t      Colormap;	/* source colormap */
Pixrect        *pPrTmp;		/* a temporary subimage-sized pixrect */
FILE           *fp;		/* opened script */

/* rip.h token */
char           *pUsage[] = {
  "ReadImage", "ImageFileName",					/* 0 */
  "ReadKernel", "KernelFileName",				/* 1 */
  "Convolve", "ImageFileName KernelFileName BlockFactor",	/* 2 */
  "OpenWindow", "WindowLabel Xsize Ysize",			/* 3 */
  "AddServer", "ServerName",					/* 4 */
  "Help", "",							/* 5 */
NULL, NULL};

main(argc, argv)
  int             argc;
  char           *argv[];
{
  char            args[MAXARGS][MAXSTR];

  if ((argc != 4) || !(fp = fopen(argv[1], "r"))) {
    fprintf(stderr, "Usage: %s filename xsize ysize\n", argv[0]);
    exit(-1);
  }
  pRequest = (Packet *) malloc(sizeof(Packet));
  pReply = (Packet *) malloc(sizeof(Packet));

  /*
   * Open a window, click to advance thru commands.  SunView is the front-end
   * for all the processing, managing mouse events in the window.
   * OpenWindow() remains as an rip operation, but this version doesn't
   * support more than the one main() window.
   */
  strcpy(args[0], "OpenWindow");
  strcpy(args[1], argv[0]);
  strcpy(args[2], argv[2]);
  strcpy(args[3], argv[3]);
  OpenWindow(3, args);
}

/*
 * ReadRIP() - read a command line, clean-up & parse it in, then attempt to
 * fill a Request packet.  Note I avoided stdin, as SunView signals
 * complicate its use!
 */
ReadRIP()
{
  int             i, op = 0;
  char            line[MAXSTR];
  char            args[MAXARGS][MAXSTR];
  static int      line_num = 0;
  int             arg_count = 0;

  if (!fgets(line, MAXSTR, fp))
    exit(0);
  printf("%s", line);
  line_num++;
  RemoveComments(line);
  if (line[0] && ((arg_count = Parse(line, args)) >= 0)) {
    pRequest->op = EMPTY;
    /*
     * Establish tokens for the operation.
     */
    for (i = 0; pUsage[i] && strcmp(pUsage[i], args[0]); op++, i += 2);
    if (!pUsage[i])
      fprintf(stderr,
	      "don't know how to \"%s\" at line %d: \"%s\"\n", args[0], line_num, line);

    switch (op) {
    case READIMAGE:		/* send total image to each server */
      if (!ReadImage(arg_count, args))
	FAILED(line_num, args, line);
      RequestAll();
      break;
    case READKERNEL:		/* send kernel to each server */
      if (!ReadKernel(arg_count, args))
	FAILED(line_num, args, line);
      RequestAll();
      break;
    case CONVOLVE:		/* split & schedule up job across servers */
      if (!Convolve(arg_count, args))
	FAILED(line_num, args, line);
      SubBFS();
      break;
    case OPENWINDOW:		/* locally open a window - careful ! */
      if (!OpenWindow(arg_count, args))
	FAILED(line_num, args, line);
      break;
    case ADDSERVER:		/* locally add a server */
      if (!AddServer(arg_count, args))
	FAILED(line_num, args, line);
      break;
    case HELP:			/* locally do Help */
      if (!Help(arg_count, args))
	FAILED(line_num, args, line);
      break;
    default:
      break;
    }
  }				/* end of if non-null line, Request done (if
				 * there was one) */
  return (TRUE);
}

/*
 * SubBFS() - an operation is to be performed on an image. Break image into
 * subimages, and send subimage definition, along with the operation, to each
 * registered server.  Pixel-value results are then dumped to the open
 * window.
 */

SubBFS()
{
  Image          *pSubIm = pRequest->Packet_u.pImage;
  int             CPid;

  /*
   * Schedule-out one block of the image to be processed.
   */
  for (pSubIm->x = 0; pSubIm->x < pPr->pr_size.x; pSubIm->x += pSubIm->dx)
    for (pSubIm->y = 0; pSubIm->y < pPr->pr_size.y; pSubIm->y += pSubIm->dy) {
      /*
       * Keep all the servers busy.  Fork a process to manage I/O and
       * scheduling to the next available server, reflect allocation in
       * ServersFree and ServerPid[].  Note that process maint.  Must be
       * performed by the parent - children can't easily communicate their
       * own status and do all the logging.  Other approaches include shared
       * memory, signals or the like - again, complicated by SunView.
       */

#ifndef DEBUG			/* Using child processes if not debugging */
      while (ServersFree == 0x0) {
	/*
	 * No servers free, loop until one becomes available.
	 */
	while ((CPid = wait((union wait *) NULL)) == -1) {
	  printf("everybody's busy i guess...");
	  usleep(100000);
	}
	FreeServer(CPid);
      }
#endif
      ForkNextServer();
#ifdef DEBUG
      ServersFree = 0xff;	/* Debugging: free without concept of a child */
#endif
    }
  /*
   * Wait here to clean things up - till all children return.
   */
#ifndef DEBUG
  while ((CPid = wait((union wait *) NULL)) != -1)
    FreeServer(CPid);
#endif
}

/*
 * ForkNextServer() - Fork and start remote procedure on the server (let Unix
 * manage resource contention and detailed scheduling!). Record allocation
 * and pid in ServersFree and ServerPid[], resp.
 */
ForkNextServer()
{
  int             Server;	/* Number of who gets allocated */
  char            SMask;
  int             Parent = TRUE;

  /*
   * Direct computations to a free server.  The servers could get all
   * allocated so might have to wait for the next available server.
   */
  if (Parent) {
    for (Server = 0; Server < NumServers; Server++) {
      SMask = 0x1 << Server;
      if (SMask & ServersFree) {
	/*
	 * Allocate the free server, record !free status. Parent returns.
	 */
	ServersFree ^= SMask;
#ifdef DEBUG			/* No fork()'s fi debugging. */
	DebugTime.tv_sec = 1000;
	DebugTime.tv_usec = 0;	/* Slow things down - man rpc for help. */
	clnt_control(cl[Server], CLSET_TIMEOUT, (char *) &DebugTime);
	goto LAUNCH;
#endif
	if (ServerPid[Server] = fork())
	  return (TRUE);
	Parent = FALSE;
	break;
      }
    }
  }
  /* Children only! - unless DEBUGging */
LAUNCH:
  pReply = rip_1(pRequest, cl[Server]);
  if ((pReply == NULL) || (pReply->op == ERROR)) {
    /*
     * An error occurred while calling the server, print error message and
     * move on.
     */
    clnt_perror(cl[Server], ServerName[Server]);
    if (ServerPid[Server])
      _exit(-1);		/* Don't kill parent if debugging */
  }
  /*
   * Okay, we successfully called the remote procedure, successfully got back
   * raster data, send it to the frame buffer.  Note the data is assumed
   * organized in raster order across a given subimage, reducing client and
   * server address calulation overhead.
   */

  (char *) ((struct mpr_data *) pPrTmp->pr_data)->md_image =
    pReply->Packet_u.pImage->Data.Data_val;
  pw_write(pPw, pReply->Packet_u.pImage->x, pReply->Packet_u.pImage->y,
	   pReply->Packet_u.pImage->dx, pReply->Packet_u.pImage->dy,
	   PIX_SRC, pPrTmp, 0, 0);

  if (!Parent)
    _exit(0);			/* kill child, don't kill debugging parent. */
}

/*
 * RequestAll() - send the assembled request to each registered server.
 */
RequestAll()
{
  int             Server;
  for (Server = 0; Server < NumServers; Server++) {
    pReply = rip_1(pRequest, cl[Server]);
    if ((pReply == NULL) || (pReply->op == ERROR)) {
      clnt_perror(cl[Server], ServerName[Server]);
      _exit(-1);
    }
  }
}

/*
 * Marking servers as free from the child (in the parent's space) is
 * difficult and can cause collisions, so I do it in the parent process.
 */
FreeServer(CPid)
  int             CPid;
{
  int             i;
  char            mask = 0x1;

  for (i = 0; i < NumServers; i++)
    if (CPid == ServerPid[i])
      break;

  if (i == NumServers) {
    fprintf(stderr, "FreeServer: not my child!\n");
    return (FALSE);
  }
  mask <<= i;
  ServersFree |= mask;
  return (TRUE);
}

Parse(line, args)
  char            line[];
  char            args[MAXARGS][MAXSTR];
{
  int             FoundArg = 0;
  int             line_index = 0;
  int             arg_index = 0;
  int             char_index = 0;

  args[0][0] = NULL;		/* In case there are no args - should have
				 * avoided this. */

  while (line[line_index] != NULL) {
    /* No leading or trailing spaces exist, RemoveComments(), start new arg. */
    if (line[line_index] == ' ') {
      args[arg_index][char_index] = NULL;	/* terminate the previous arg */
      arg_index++;
      char_index = 0;
    } else {
      FoundArg++;
      args[arg_index][char_index++] = line[line_index];
    }
    line_index++;
  }
  args[arg_index][char_index] = NULL;
  if (FoundArg)
    return (arg_index);
  return (-1);
}

RemoveComments(line)	/* Comment char ';', whitespace ' ', ',', '\t' */
  char            line[];
{
  char            new_line[MAXSTR];
  char           *p_c;
  int             i = 0;
  int             j = 0;

  /* swap tabs and ',' for spaces if there are any */
  while (p_c = strchr(line, '\t'))
    *p_c = ' ';
  while (p_c = strchr(line, ','))
    *p_c = ' ';

  /* Stop at a ';', as well as remove multiple spaces */
  for (i = 0; line[i] != NULL; i++) {
    if (line[i] == ';') {
      new_line[j] = NULL;
      break;
    }
    if (!((i > 0) && (line[i] == ' ') && (line[i - 1] == ' ')))
      new_line[j++] = line[i];
  } new_line[j] = NULL;

  /* Remove leading and trailing space if they exist */
  while (new_line[0] == ' ')
    strcpy(new_line, &new_line[1]);
  j = strlen(new_line);
  for (i = 1; i < j; i++) {
    if (new_line[j - i] == ' ')
      new_line[j - i] = NULL;
    if (new_line[j - i] == '\n')
      new_line[j - i] = NULL;
    else
      break;
  }
  strcpy(line, new_line);

  return (TRUE);
}

/*
 * Operations from here on - client primitives for image processing. see
 * pUsage[][] as defined in main() for some use guidance
 */
ReadImage(argc, args)		/* Reads rasterfile image file */
  int             argc;
  char            args[MAXARGS][MAXSTR];
{
  Image          *pI;
  FILE           *fpIn;

  if (argc != 1)
    return (FALSE);

  if (!(fpIn = fopen(args[1], "r")) || !(pPr = pr_load(fpIn, &Colormap))) {
    fprintf(stderr, "cannot load image \"%s\"\n", args[1]);
    return (FALSE);
  }
  fclose(fpIn);

  /* Stuff it in the window */
  pw_rop(pPw, 0, 0, pPr->pr_size.x, pPr->pr_size.y,
	 PIX_SRC, pPr, 0, 0);

  /* Go to end of list and add an image */
  if (!pImage) {
    pI = pImage = (Image *) malloc(sizeof(Image));
  } else {
    TRAVERSE(pI, pImage);
    pI->pNext = (Image *) malloc(sizeof(Image));
    pI = pI->pNext;
  }
  pI->pNext = NULL;

  /* Put the data into the Image structure for a local copy */
  pI->sName = strdup(args[1]);
  pI->x = pI->y = 0;
  pI->dx = pPr->pr_size.x;
  pI->dy = pPr->pr_size.y;
  pI->Data.Data_len = pPr->pr_size.x * pPr->pr_size.y * pPr->pr_depth / 8;
  /* volatile - SHOULD REALLY COPY IMAGE */
  pI->Data.Data_val =
    (char *) ((struct mpr_data *) pPr->pr_data)->md_image;

  /* Build up Request packet - XDR handles struct ptrs... */
  pRequest->op = READIMAGE;
  pRequest->Packet_u.pImage = pI;
  return (TRUE);
}

/* Reads a kernel file - square, asymmetric, non-separable filters ASSumed */
ReadKernel(argc, args)
  int             argc;
  char            args[MAXARGS][MAXSTR];
{
  FILE           *fpIn;
  Kernel         *pK;
  int             i, j, k;

  if (argc != 1)
    return (FALSE);

  if (!(fpIn = fopen(args[1], "r"))) {
    fprintf(stderr, "cannot load kernel \"%s\"\n", args[1]);
    return (FALSE);
  }
  /* Go to end of list and add a kernel */
  if (!pKernel) {
    pK = pKernel = (Kernel *) malloc(sizeof(Kernel));
  } else {
    TRAVERSE(pK, pKernel);
    pK->pNext = (Kernel *) malloc(sizeof(Kernel));
    pK = pK->pNext;
  }
  pK->pNext = NULL;

  /* Read data file in */
  pK->Value.Value_val = (float *) malloc(sizeof(float) * MAXKER);
  for (k = 0; (fscanf(fpIn, "%f", &pK->Value.Value_val[k]) == 1); k++) {
    if (k >= MAXKER) {
      fprintf(stderr, "%s: kernel too big\n", args[1]);
      return (FALSE);
    }
  }
  fclose(fpIn);

  /* Update the local kernel list */
  pK->sName = strdup(args[1]);
  pK->xsize = pK->ysize = rint(sqrt((double) k));
  pK->Value.Value_len = k;	/* XDR only send this much, not MAXKER */

  /* Build up Request packet */
  pRequest->op = READKERNEL;
  pRequest->Packet_u.pKernel = pK;
  return (TRUE);
}

AddServer(argc, args)		/* Attempt to reset & add another server to
				 * the list */
  int             argc;
  char            args[MAXARGS][MAXSTR];
{
  int             i;

  if (argc != 1)
    return (FALSE);

  /*
   * Create another client "handle" used for calling MESSAGEPROG on servers
   * designated on command line.  We tell RPC package to use the "tcp"
   * protocol when contacting the server
   */
  if (NumServers >= MAXSERVERS) {
    fprintf(stderr, "%s: too many servers, max is %d\n",
	    args[0], MAXSERVERS);
    return (NULL);
  }
  /*
   * Build-up Serverxxx information, attempt to reset the server, qualify
   * candidates.
   */
  cl[NumServers] = clnt_create(args[1], RIPPROG, RIPVERS, "tcp");
  if (!cl[NumServers]) {
    /*
     * Couldn't establish connection with server, print error message.
     */
    clnt_pcreateerror(args[1]);
    return (NULL);
  }
  /*
   * Attempt to initialize the server, to check its health.  I throw away the
   * returned server hostname.
   */
  pRequest->op = ADDSERVER;
  pRequest->Packet_u.sOp = strdup(args[1]);
  pReply = rip_1(pRequest, cl[NumServers]);
  if (!pReply || (pReply->op != ADDSERVER)) {
    fprintf(stderr, "%s: could not reset %s\n", args[0],
	    ServerName[NumServers]);
    return (FALSE);
  }
  /*
   * This server is okay, register it.
   */
  strcpy(ServerName[NumServers], args[1]);
  ServersFree <<= 1;
  ServersFree |= 0x1;
  NumServers++;
  return (TRUE);
}

Convolve(argc, args)		/* Convolve an image w/ a kernel */
  int             argc;
  char            args[MAXARGS][MAXSTR];
{
  char           *sBuf[MAXSTR];
  Image          *pSubIm;

  if (argc != 3)
    return (FALSE);

  /* Build up Request packet - passing image and kernel names is poorly done */
  pRequest->op = CONVOLVE;
  sprintf(sBuf, "%s %s", args[1], args[2]);

  /*
   * Define the subimage size.  Always zero the opaque data lengths and
   * unused struct pointers - XDR looks at them!
   */
  pSubIm = pRequest->Packet_u.pImage = (Image *) malloc(sizeof(Image));
  pRequest->Packet_u.pImage->Data.Data_len = 0;
  pSubIm->sName = strdup(sBuf);
  pSubIm->pNext = NULL;
  pSubIm->dx = pSubIm->dy = pPr->pr_size.x / atoi(args[3]);	/* square! */

  /* Update the local working subimage pixrect */
  if (pPrTmp)
    mem_destroy(pPrTmp);
  pPrTmp = mem_create(pSubIm->dx, pSubIm->dy, 8);

  return (TRUE);
}

OpenWindow(argc, args)		/* Open a SunView window and loop on it */
  int             argc;
  char            args[MAXARGS][MAXSTR];
{
  int             i;
  u_char          gray[256];
  Frame           frame;
  Canvas          canvas;

  if (argc != 3)
    return (FALSE);

  /* Create tool and get a hold of its pixwin, install mouse catcher */
  frame = window_create((Frame) NULL, FRAME, FRAME_LABEL, args[1], 0);
  canvas = window_create(frame, CANVAS, WIN_WIDTH, atoi(args[2]),
		 WIN_HEIGHT, atoi(args[3]), WIN_EVENT_PROC, MouseCaught, 0);
  window_fit(frame);
  pPw = canvas_pixwin(canvas);


  /* Build and install a ramp color table if the image doesn't have one */
  if ((Colormap.type == RMT_NONE) || !Colormap.length) {
    for (i = 0; i < 256; i++)
      gray[i] = i;
    pw_setcmsname(pPw, "ramp");
    Colormap.type = RMT_EQUAL_RGB;
    Colormap.length = 256;
    Colormap.map[0] = Colormap.map[1] = Colormap.map[2] = gray;
  } else {
    pw_setcmsname(pPw, "original");
  }
  pw_putcolormap(pPw, 0, Colormap.length, Colormap.map[0],
		 Colormap.map[1], Colormap.map[2]);

  /*
   * Watch-out - indiscriminate mixing of signals with the dispatcher is
   * fatal!  Read the SunView Programmer's Guide.
   */
  window_main_loop(frame);	/* never returns... */
}

/*
 * MouseCaught() - event procedure called by SunView Notifier. Click the left
 * button to consume an rip command.
 */
MouseCaught(canvas, event, arg)
  Canvas          canvas;
  Event          *event;
  caddr_t         arg;
{
  int             i, j;
  if ((event_action(event) == MS_LEFT) && event_is_up(event)) {
    ReadRIP();			/* Process one command from script */
  }
}

Help(argc, args)
  int             argc;
  char            args[MAXARGS][MAXSTR];
{
  int             zz;
  fprintf(stderr, "commands include:\n");
  for (zz = 0; pUsage[zz] != NULL; zz += 2)
    fprintf(stderr, "%s %s\n", pUsage[zz], pUsage[zz + 1]);
  return (TRUE);
}
