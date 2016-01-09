/* rip_ops.c - client primitives for image processing.  see pUsage[][]
   as defined in main() for some use guidance */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <suntool/sunview.h>
#include <suntool/canvas.h>
#include <rpc/rpc.h>
#include <signal.h>
#include "rip.h"
#include "rip_shared.h"

/* RPC and scheduling overhead */
extern char ServersFree;
extern int  NumServers;               
extern char ServerName[MAXSERVERS][32];
extern CLIENT *cl[MAXSERVERS]; 
extern Packet *pRequest;
extern Packet *pReply;

/* application-specific local lists */
extern Kernel  *pKernel  ;
extern Image   *pImage   ;

/* local window overhead */
extern Pixwin *pPw;
extern colormap_t Colormap;
extern Pixrect *pPr;
extern Pixrect *pPrTmp;

/* operator look-up */
extern char    *pUsage[];

ReadImage(argc, args)    /* reads rasterfile image file */
int argc;
char args[MAXARGS][MAXLEN];
  {
  Image *pI;
  FILE *fpIn;
 
  if (argc != 1) return(FALSE);

  if (!(fpIn=fopen(args[1],"r"))||!(pPr=pr_load(fpIn, &Colormap)))
    { fprintf(stderr, "cannot load image \"%s\"\n", args[1]);
      return(FALSE); }
  fclose(fpIn);
  
  /* stuff it in the window */
  pw_rop(pPw, 0, 0, pPr->pr_size.x, pPr->pr_size.y,
    PIX_SRC, pPr, 0, 0);
 
  /* go to end of list and add an image */
  if (!pImage) {
    pI  = pImage = (Image *)malloc(sizeof(Image));
  } else {
    TRAVERSE(pI, pImage);
    pI->pNext = (Image *)malloc(sizeof(Image));
    pI = pI->pNext;
  }
  pI->pNext = NULL;

  /* put the data into the Image structure for a local copy */
  pI->sName = strdup(args[1]);
  pI->x = pI->y = 0;
  pI->dx = pPr->pr_size.x;
  pI->dy = pPr->pr_size.y;
  pI->Data.Data_len = pPr->pr_size.x*pPr->pr_size.y*pPr->pr_depth/8;
  /* volatile - SHOULD REALLY COPY IMAGE */
  pI->Data.Data_val =
	(char *) ((struct mpr_data *)pPr->pr_data)->md_image;

  /* build up Request packet - XDR handles struct ptrs... */
  pRequest->op = READIMAGE;
  pRequest->Packet_u.pImage = pI;
  return (TRUE);
  }
 
ReadKernel(argc, args)  /* reads a kernel file */
int argc;
char args[MAXARGS][MAXLEN];
  {
  FILE *fpIn;
  Kernel *pK;
  int i, j, k;
 
  if (argc != 1) return(FALSE);
 
  if (!(fpIn=fopen(args[1],"r")))
    { fprintf(stderr, "cannot load kernel \"%s\"\n", args[1]);
      return(FALSE); }
 
  /* go to end of list and add a kernel */
  if (!pKernel) {
    pK  = pKernel = (Kernel *)malloc(sizeof(Kernel));
  } else {
    TRAVERSE(pK, pKernel);
    pK->pNext = (Kernel *)malloc(sizeof(Kernel));
    pK = pK->pNext;
  }
  pK->pNext = NULL;

  /* read data file in */
  pK->Value.Value_val = (float *) malloc(sizeof(float)*MAXKER);
  for (k=0; (fscanf(fpIn, "%f", &pK->Value.Value_val[k]) == 1); k++) {
    if (k>=MAXKER) {
      fprintf(stderr, "%s: kernel too big\n", args[1]);
      return (FALSE);
    }
  }
  fclose(fpIn);
  
  /* update the local kernel list */
  pK->sName = strdup(args[1]);
  pK->xsize = pK->ysize = rint(sqrt((double)k));
  pK->Value.Value_len = k; /* XDR only send this much, not MAXKER */

  /* build up Request packet */
  pRequest->op = READKERNEL;
  pRequest->Packet_u.pKernel = pK;
  return (TRUE);
  }

AddServer(argc, args)	/* attempt to add another server to the list */
  int argc;
  char args[MAXARGS][MAXLEN];
{
  int i;

  if (argc != 1) return(FALSE);

  /*
   * Create another client "handle" used for calling MESSAGEPROG on
   * servers designated on command line.  We tell RPC package
   * to use the "tcp" protocol when contacting the server
   */
  if (NumServers >= MAXSERVERS) {
  	fprintf(stderr, "%s: too many servers, max is %d\n",
      		args[0], MAXSERVERS);
	return(NULL);
  }
  /*
   * Build-up Serverxxx information, attempt to
   * reset the server, qualify candidates.
   */
  cl[NumServers] = clnt_create(args[1], RIPPROG, RIPVERS, "tcp");
  if (!cl[NumServers]) {
    /*
     * Couldn't establish connection with server, print error message.
     */
    clnt_pcreateerror(args[1]); return(NULL);
  }

  /*
   * Attempt to initialize the server, to check its health.
   */
  pRequest->op = ADDSERVER;
  pRequest->Packet_u.sOp = strdup(args[1]);
  pReply = rip_1(pRequest,cl[NumServers]);
  if (!pReply || (pReply->op != SUCCESS)) {
    fprintf(stderr, "%s: could not reset %s\n", args[0],
      ServerName[NumServers]);
    return(FALSE);
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


Convolve(argc, args)
int argc;
char args[MAXARGS][MAXLEN];
{
  char *sBuf[MAXSTR];
  Image *pSubIm;

  if (argc != 3) return(FALSE);

  /* build up Request packet - passing image and kernel names is poorly done */
  pRequest->op = CONVOLVE;
  sprintf(sBuf, "%s %s", args[1], args[2]);

  /* define the subimage size */
  pSubIm = pRequest->Packet_u.pImage = (Image *)malloc(sizeof(Image));
  pSubIm->sName = strdup(sBuf);
  pSubIm->pNext = NULL; /* XDR goes crazy if you don't...  */
  pSubIm->dx = pSubIm->dy = pPr->pr_size.x/atoi(args[3]); /*square!*/

  /* update the local subimage pixrect */
  if (pPrTmp) mem_destroy(pPrTmp);
  pPrTmp = mem_create(pSubIm->dx, pSubIm->dy, 8);

  return (TRUE);
}

OpenWindow(argc, args)
int argc;
char args[MAXARGS][MAXLEN];
{
  int i;
  u_char gray[256];
  Frame frame;
  Canvas canvas;

  if (argc != 3) return(FALSE);

  /* create tool and get a hold of its pixwin, install mouse catcher */
  frame = window_create((Frame)NULL, FRAME,
    FRAME_LABEL, args[1], 0);
  canvas = window_create(frame, CANVAS,
    WIN_WIDTH, atoi(args[2]), WIN_HEIGHT, atoi(args[3]), 
    WIN_EVENT_PROC, MouseCaught, 0);
  window_fit(frame);
  pPw = canvas_pixwin(canvas);


  /* build and install a ramp color table if the image doesn't have one */
  if ((Colormap.type == RMT_NONE) || !Colormap.length) {
    for (i=0; i<256; i++) gray[i] = i;
    pw_setcmsname(pPw, "ramp");
    Colormap.type = RMT_EQUAL_RGB;
    Colormap.length = 256;
    Colormap.map[0] = Colormap.map[1] = Colormap.map[2] = gray;
    }
  else {
    pw_setcmsname(pPw, "original");
    }
  pw_putcolormap(pPw, 0, Colormap.length, Colormap.map[0],
  Colormap.map[1], Colormap.map[2]);
	
  /*
   * Watch-out - indiscriminate mixing of signals with the
   * dispatcher is fatal!  Read the SunView Programmer's Guide.
   */
  window_main_loop(frame); /* never returns... */
}

/*
 * MouseCaught() - event procedure called by SunView Notifier.
 * Click the left button to consume an RIP command.
 */
MouseCaught(canvas, event, arg)
Canvas canvas;
Event *event;
caddr_t arg;
{
  int i, j;
  if ((event_action(event) == MS_LEFT) && event_is_up(event)) {
    ReadRIP(); /* process one command */
  }
}

Help(argc, args)
int argc;
char args[MAXARGS][MAXLEN];
{
  int zz;
  fprintf(stderr,"commands include:\n");
  for(zz=0; pUsage[zz] != NULL; zz+=2)
    fprintf(stderr,"%s %s\n",pUsage[zz],pUsage[zz+1]);
  return (TRUE);
}
