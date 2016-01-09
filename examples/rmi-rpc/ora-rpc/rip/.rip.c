/*
 * rip.c: remote image processing client main
 */
#include <stdio.h>
#include <rpc/rpc.h>
#include <string.h>
#include <math.h>
#include <sys/wait.h>
#include <pixrect/pixrect_hs.h>
#include <suntool/sunview.h>
#include <suntool/canvas.h>
#include "rip.h"
#include "rip_shared.h"

/*
 * global server/client relationship stuff - max servers <= 8
 */
char ServersFree=0x0;           /* bits are 1 if free, 0 if busy */
int  NumServers;                /* total # of servers connected */
int  ServerPid[MAXSERVERS];     /* used to record fork()'ed pids */
int  ServerUse[MAXSERVERS];     /* record server use */
char ServerName[MAXSERVERS][32];/* record server names */
CLIENT *cl[MAXSERVERS];         /* client handles */

/*
 * global graphic/display stuff
 */
Kernel	*pKernel = NULL;/* list of active kernels */
Image	*pImage = NULL;	/* list of active images */
Pixwin	*pPW;		/* the window canvas for the data */
colormap_t Colormap;    /* to put the source colormap in */
int DisplayMesh = FALSE;/* show the distortion mesh */
Frame frame;      	/* the base frame display window */
Pixrect *pPrTmp; /* a temporary subimage-sized pixrect */

/*
 * client <--> server data structures, defined in protocol
 */
Request *pRequest;
Reply *pReply;    

/*
 * functions and procedures in this file
 */
void	FreeServer();
void	ForkNextServer();
						/* rip.h token */
char	*pUsage[] = {
  "ReadImage", "ImageFileName",			/* 0 */
  "ReadKernel", "KernelFileName",		/* 1 */
  "Convolve", "ImageFileName KernelFileName",	/* 2 */
  "OpenWindow", "WindowLabel Xsize Ysize",	/* 3 */
  "AddServer", "ServerName",			/* 4 */
  "Help", "",					/* 5 */
  NULL, NULL };

main(argc, argv)
int argc;
char *argv[];
{
  char line[MAXLEN];
  char args[MAXARGS][MAXLEN];
  int line_num = 0;
  int arg_count = 0;
  int i, op;
  
  pRequest = (Request *)malloc(sizeof(Request));
  pReply = (Reply *)malloc(sizeof(Reply));
  pPrTmp = mem_create(pPr->pr_size.x/BLOCKFACT,
    pPr->pr_size.y/BLOCKFACT, 8);
  
  /*
   * Read commands from stdin, clean-up then parse them in,
   * then attempt to fill a Request packet, failure
   * should return a NULL.
   */
  while (gets(line)) {
    op = 0;
    line_num++;
    RemoveComments(line);
    if (line[0] && ((arg_count = Parse(line, args)) >= 0)) {
      pRequest->op = EMPTY;
      /*
       * establish tokens for the operation.
       */
      for(i=0; pUsage[i] && strcmp(pUsage[i],args[0]); op++, i+=2);
      if (!pUsage[i]) fprintf(stderr,
	  "%s: don't know how to \"%s\" at line %d: \"%s\"\n", 
          argv[0], args[0], line_num, line);

      switch (op) {
	case READIMAGE: /* send total image to each server */
	  if (ReadImage(arg_count, args)) RequestAll();
	  break;
	case READKERNEL: /* send kernel to each server */
	  if (ReadKernel(arg_count, args)) RequestAll();
	  break;
	case CONVOLVE: /* split & schedule up job across servers */
	  SubSchedule();
      }
    } /* end of if non-null line, Request done (if there was one) */
  } /* end of while */
} /* end of main */

/*
 * SubSchedule() - an operation is to be performed on an image.
 * Break image into subimages, and send subimage definition,
 * along with the operation, to each registered server.
 * Pixel-value results are then dumped to the open window.
 */
void SubSchedule()
{
  Image SubImage;
  int CPid;               /* dead child pid */
  
  pRequest->Packet_u.pImage = &SubImage;
  SubImage.dx = SubImage.dy = pPr->pr_size.x/BLOCKFACT;

  /*
   * Schedule-out one block of the image to be processed.
   */
  for(SubImage.x=0;SubImage.x<pPr->pr_size.x;SubImage.x+=SubImage.dx)
   for(SubImage.y=0;SubImage.y<pPr->pr_size.y;SubImage.y+=SubImage.dy)
    /*
     * Keep all the servers busy, with a process
     * for a Request forked to them. Fork a process to manage I/O and
     * scheduling to the next available server, reflect allocation in
     * ServersFree and ServerPid[].
     */
    {
    while (ServersFree == 0x0) {
      /*
       * No servers are free, loop indefinitely checking to see when
       * a server becomes available.
       */
      while ((CPid=wait((union wait *)NULL))==-1) {
	printf("wait loop..."); usleep(100000); }
      FreeServer(CPid);
      }
    ForkNextServer();
    while ((CPid=wait((union wait *)NULL))!=-1)
      FreeServer(CPid);
}

/*
 * ForkNextServer() - Fork and start remote procedure on the server
 * (let unix manage resource contention and detailed scheduling!).
 * Record allocation and pid in ServersFree and ServerPid[], resp.
 */
void ForkNextServer()
{
  int Server;    /* number of who gets allocated */
  char SMask;

  /*
   * Direct computations to a free server.
   * I don't believe we could ever get all allocated
   * while we weren't looking, but lets check and
   * wait for an available server.
   */
  Server=NumServers;

  while (Server==NumServers) {
    for (Server=0; Server<NumServers; Server++) {
      SMask = 0x1 << Server;
      if (SMask & ServersFree) {
        /* 
         * Free server, allocate it and record CPid and !free status.
         * If its the parents thread thru here, return
         * after recording CPid, otherwise go forth.
         */
        ServersFree ^= SMask;
        if (ServerPid[Server]=fork()) return();
        break;
      }
    }
  }

  /* children only! */
  pReply = rip_1(pRequest, cl[Server]);
  /* parent has to free server */
  if ((pReply == NULL) || (pReply->op == ERROR)) {
    /*
     * An error occurred while calling the server,
     * print error message and move on...
     */
    clnt_perror(cl[Server], ServerName[Server]);
    _exit(-1);
  }
  
  /*
   * Okay, we successfully called the remote procedure,
   * successfully got back raster data, send it to the frame buffer.
   * Note the data is assumed organized in raster order 
   * across a given subimage, reducing client address
   * calulation overhead.
   */
  (char *) ((struct mpr_data *)pPrTmp->pr_data)->md_image =
    pReply->Packet_u->pImage.Data.Data_val;
   pw_write(pPW,
     pReply->Packet_u->pImage.x, pReply->Packet_u->pImage.y,
     pReply->Packet_u->pImage.dx, pReply->Packet_u->pImage.dy,
     PIX_SRC, pPrTmp, 0, 0);

  /*
   * Child process exits, status variable is changed back
   * in main() in response to a wait().  Dump results
   * to window.
   */
  _exit(0);
}
