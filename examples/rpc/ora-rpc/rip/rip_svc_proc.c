/*
 * rip_svc_proc.c: rip remote server procedure
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <rpc/rpc.h>
#include "rip.h"
#include "rip_shared.h"
/*
 * Copies of image processing data (lists).
 */
static Kernel  *pKernel = NULL;	/* list of active kernels */
static Image   *pImage = NULL;	/* list of active images */

/*
 * rip_1() - server procedure to execute one of many rip operations. As the
 * protocol suggests, Request Packets are returned with some error logging.
 */
Packet         *
rip_1(pRequest)
  Packet         *pRequest;
{
  /*
   * Allocate static space for the return data.
   */
  static Packet   Reply;
  char            sBuf[MAXSTR];

  /*
   * Reset last result & check to see what operation is Requested.  I preceed
   * the functions w/ an 'S' to avoid function name collision when client and
   * server routines are locally linked for debugging. This is a good way to
   * start testing as there's no XDR formatting. Once you have things
   * working, THEN include the XDR stuff, and tackle those problems.
   */
  Reply.op = ERROR;
  switch (pRequest->op) {
  case READIMAGE:		/* send total image to each server */
    SReadImage(pRequest, &Reply);
    break;
  case READKERNEL:		/* send kernel to each server */
    SReadKernel(pRequest, &Reply);
    break;
  case CONVOLVE:		/* split & schedule up job across servers */
    SConvolve(pRequest, &Reply);
    break;
  case ADDSERVER:		/* locally add a server */
    SAddServer(pRequest, &Reply);
    break;
  default:			/* below is the kind of thing you should do
				 * in each function */
    gethostname(sBuf, MAXSTR);
    strcat(sBuf, ": don't understand operation");
    Reply.Packet_u.sOp = strdup(sBuf);
    Reply.op = SUCCESS;
  }
  return (&Reply);
}

/*
 * Server operations for image processing from here on out. start with:
 * SReadImage() - store a copy of this image in the linked list.
 */
SReadImage(pRequest, pReply)
  Packet         *pRequest, *pReply;
{
  Image          *pI;

  /* Go to end of list and add an image */
  if (!pImage) {
    pI = pImage = (Image *) malloc(sizeof(Image));
  } else {
    TRAVERSE(pI, pImage);
    pI->pNext = (Image *) malloc(sizeof(Image));
    pI = pI->pNext;
  }
  pI->pNext = NULL;

  /* Put the data into the Image structure for a local copy as it's volatile */
  bcopy(pRequest->Packet_u.pImage, pI, sizeof(Image));
  pI->sName = strdup(pRequest->Packet_u.pImage->sName);
  pI->Data.Data_val = (char *) malloc((int) pI->Data.Data_len);
  bcopy(pRequest->Packet_u.pImage->Data.Data_val, pI->Data.Data_val,
	pI->Data.Data_len);

  /* Build up Reply packet */
  pReply->op = SUCCESS;
}

/*
 * SReadKernel() - store a copy of this kernel in the linked list.
 */
SReadKernel(pRequest, pReply)
  Packet         *pRequest, *pReply;
{
  Kernel         *pK;

  /* Go to end of list and add a kernel */
  if (!pKernel) {
    pK = pKernel = (Kernel *) malloc(sizeof(Kernel));
  } else {
    TRAVERSE(pK, pKernel);
    pK->pNext = (Kernel *) malloc(sizeof(Kernel));
    pK = pK->pNext;
  }
  pK->pNext = NULL;

  /* Put the data into the Kernel structure for a local copy */
  pK->sName = strdup(pRequest->Packet_u.pKernel->sName);
  pK->xsize = pRequest->Packet_u.pKernel->xsize;
  pK->ysize = pRequest->Packet_u.pKernel->ysize;
  pK->Value.Value_len = pRequest->Packet_u.pKernel->Value.Value_len;
  pK->Value.Value_val = (float *) malloc((int) pK->Value.Value_len * sizeof(float));
  bcopy(pRequest->Packet_u.pKernel->Value.Value_val,
	pK->Value.Value_val, pK->Value.Value_len * sizeof(float));

  pReply->op = SUCCESS;
}

/*
 * SConvolve() - perform convolution between a subimage of the last mentioned
 * image and kernel.
 */
SConvolve(pRequest, pReply)
  Packet         *pRequest, *pReply;
{
  char            sk[MAXSTR], si[MAXSTR];
  Kernel         *pK;
  Image          *pI;
  char           *pSubImage, *pMAC;
  float           f;
  int             x, xx, y, yy, dx, dxx, dy, dyy;
  int             i, dxImage, SubImagePixelOffset;

  /*
   * Try to find the kernel and the image mentioned, otherwise, use the last
   * one there...
   */
  sscanf(pRequest->Packet_u.pImage->sName, "%s%s", si, sk);
  TRAVERSE(pK, pKernel) if (!strcmp(pK->sName, sk))
    break;
  TRAVERSE(pI, pImage) if (!strcmp(pI->sName, si))
    break;

  /* Put the reply skeleton together */
  pReply->op = CONVOLVE;
  bcopy(pRequest, pReply, sizeof(Packet));
  dx = pRequest->Packet_u.pImage->dx;
  dy = pRequest->Packet_u.pImage->dy;
  pReply->Packet_u.pImage->Data.Data_len = dx * dy;
  pMAC = pReply->Packet_u.pImage->Data.Data_val =
    (char *) calloc(dx * dy, sizeof(unsigned char));
  pReply->Packet_u.pImage->sName =
    strdup(pRequest->Packet_u.pImage->sName);
  pReply->Packet_u.pImage->pNext = NULL;

  /*
   * Make the subimage copy larger - to control edge effects of the
   * convolution. Should really preserve derivative information instead of
   * zeroing...
   */
  dxx = dx + pK->xsize;
  dyy = dy + pK->ysize;
  dxImage = pI->dx;
  pSubImage = (char *) calloc(dxx * dyy, sizeof(unsigned char));
  /* copy image a line at a time */
  for (y = pRequest->Packet_u.pImage->y, yy = (pK->ysize >> 1);
       y < pRequest->Packet_u.pImage->y + dy; y++, yy++)
    bcopy(pI->Data.Data_val + y * dxImage + pRequest->Packet_u.pImage->x,
	  pSubImage + yy * dxx + (pK->xsize >> 1), dx);

  /*
   * Step thru the defined subimage and do a convolution.  Personally, I find
   * it conceptually easier to move thru each convolution window of the
   * subimage in raster-scan order, organized like the kernel, doing the
   * multiplication and accumulation in that order.
   */

  /* Foreach non-edge pixel in SubImage(x,y)... */
  for (y = (pK->ysize >> 1); y < dy + (pK->ysize >> 1); y++) {
    for (x = (pK->xsize >> 1); x < dx + (pK->xsize >> 1); x++) {
      /* Make a scan-line version of the conv. window (like pK) */
      i = 0;
      f = 0;
      for (yy = y - (pK->ysize >> 1); yy < y + (pK->ysize >> 1); yy++) {
	for (xx = x - (pK->xsize >> 1); xx < x + (pK->xsize >> 1); xx++) {
	  SubImagePixelOffset = yy * dxx + xx;
	  f += pK->Value.Value_val[i++] *
	    (float) ((unsigned char) *(pSubImage + SubImagePixelOffset));
#ifdef VERBOSEDEBUG
	  printf("pixel:%f kernel:%f product:%d ", fConvWin[i],
		 pK->Value.Value_val[i], (unsigned) pMAC);
#endif
	}
      }
      *pMAC = (unsigned char) rint(f);
      pMAC++;
    }
  }
  return (TRUE);
}

/*
 * SAddServer() - return hostname (free some rip memory too).
 */
SAddServer(pRequest, pReply)
  Packet         *pRequest, *pReply;
{
  char            sBuf[MAXSTR];
  Image          *pI, *pIl;
  Kernel         *pK, *pKl;

  /* Blast the images and kernels */
  for (pI = pImage; pI; pI = pI->pNext, free(pIl)) {
    pIl = pI;
  }
  for (pK = pKernel; pK; pK = pK->pNext, free(pKl)) {
    pKl = pK;
  }

  /* Tell the client who you really are... */
  gethostname(sBuf, MAXSTR);
  pReply->Packet_u.sOp = strdup(sBuf);
  pReply->op = ADDSERVER;
}
