/* rip_svc_ops.c - server operations for image processing */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <rpc/rpc.h>
#include "rip.h"
#include "rip_shared.h"

/*
 * Copies of image processing data (lists).
 */
static Kernel *pKernel = NULL; /* list of active kernels */
static Image *pImage = NULL; /* list of active images */

/*
 * SReadImage() - store a copy of this image in the linked list.
 */
SReadImage(pRequest, pReply)
Packet *pRequest, *pReply;
{
  Image *pI;
 
  /* go to end of list and add an image */
  if (!pImage) {
    pI  = pImage = (Image *)malloc(sizeof(Image));
  } else {
    TRAVERSE(pI, pImage);
    pI->pNext = (Image *)malloc(sizeof(Image));
    pI = pI->pNext;
  }
  pI->pNext = NULL;

  /* put the data into the Image structure for a local copy 
     could be volatile */
  bcopy(pRequest->Packet_u.pImage, pI, sizeof(Image));
  pI->sName = strdup(pRequest->Packet_u.pImage->sName);
  pI->Data.Data_val = (char *)malloc((int)pI->Data.Data_len);
  bcopy(pRequest->Packet_u.pImage->Data.Data_val, pI->Data.Data_val,
  pI->Data.Data_len);

  /* build up Reply packet */
  pReply->op = SUCCESS;
}

/*
 * SReadKernel() - store a copy of this kernel in the linked list.
 */
SReadKernel(pRequest, pReply)
Packet *pRequest, *pReply;
{
  Kernel *pK;

  /* go to end of list and add a kernel */
  if (!pKernel) {
    pK  = pKernel = (Kernel *)malloc(sizeof(Kernel));
  } else {
    TRAVERSE(pK, pKernel);
    pK->pNext = (Kernel *)malloc(sizeof(Kernel));
    pK = pK->pNext;
  }
  pK->pNext = NULL;

  /* put the data into the Kernel structure for a local copy */
  pK->sName = strdup(pRequest->Packet_u.pKernel->sName);
  pK->xsize = pRequest->Packet_u.pKernel->xsize;
  pK->ysize = pRequest->Packet_u.pKernel->ysize;
  pK->Value.Value_len = pRequest->Packet_u.pKernel->Value.Value_len;
  pK->Value.Value_val = (float *)malloc((int)pK->Value.Value_len*sizeof(float));
  bcopy(pRequest->Packet_u.pKernel->Value.Value_val,
    pK->Value.Value_val, pK->Value.Value_len*sizeof(float));

  pReply->op = SUCCESS;
}

/*
 * SConvolve() - perform convolution between a subimage of the
 * last mentioned image and kernel.
 */
SConvolve(pRequest, pReply)
Packet *pRequest, *pReply;
{
  char sk[MAXSTR], si[MAXSTR];
  Kernel *pK;
  Image *pI;
  char *pSubImage, *pMAC;
  float fConvWin[MAXKER];
  int x, xx, y, yy, dx, dxx, dy, dyy;
  int i, dxImage, SubImagePixelOffset;

  /*
   * try to find the kernel and the image mentioned,
   * otherwise, use the last one there...
   */
  sscanf(pRequest->Packet_u.pImage->sName, "%s%s", si, sk);
  TRAVERSE(pK, pKernel) if (!strcmp(pI->sName, sk)) break;
  TRAVERSE(pI, pImage) if (!strcmp(pI->sName, si)) break;
  
  /* put the reply skeleton together */
  pReply->op = CONVOLVE;
  bcopy (pRequest, pReply, sizeof(Packet));
  dx = pRequest->Packet_u.pImage->dx;
  dy = pRequest->Packet_u.pImage->dy;
  pReply->Packet_u.pImage->Data.Data_len = dx*dy;
  pMAC = pReply->Packet_u.pImage->Data.Data_val = 
    (char *)malloc(dx*dy*sizeof(char));

  /*
   * Make the subimage copy larger - to control edge effects
   * of the convolution.  Should really preserve derivative
   * information instead of zeroing...
   */
  dxx = dx + pK->xsize;
  dyy = dy + pK->ysize;
  dxImage = pI->dx;
  pSubImage = (char *)calloc(dxx*dyy*sizeof(char));
  /* copy image a line at a time */
  for (y=pRequest->Packet_u.pImage->y, yy=(pK->ysize<<1);
    y<pRequest->Packet_u.pImage->y+dy; y++, yy++)
    bcopy(pI->Data.Data_val+y*dxImage+pRequest->Packet_u.pImage->x,
      pSubImage+yy*dxx+(pK->xsize<<1), dx);
  
  /*
   * Step thru the defined subimage and do a convolution.
   * Personally, I find it conceptually easier to break-out
   * each convolution window of the subimage, in raster-scan order, 
   * like the kernel, then do the multiplication and
   * accumulation.
   */
  /* foreach non-edge pixel in SubImage(x,y)... */
  for (y=(pK->ysize<<1); y<dy+(pK->ysize<<1); y++) {
    for (x=(pK->xsize<<1); x<dx+(pK->xsize<<1); x++) {
    /* make a scan-line version of the conv. window (like pK) */
    i = 0;
      for (yy=y-(pK->ysize<<1); yy<y+(pK->ysize<<1); yy++) {
        for (xx=x-(pK->xsize<<1); xx<x+(pK->xsize<<1); xx++) {
          SubImagePixelOffset = yy*dxx + xx;
         /* do copy manually as it's typically small */
         fConvWin[i++] = (float) * (pSubImage+SubImagePixelOffset);
        }
      }

    /* perform the convolution, store results in Reply */
    for (i=0; i<pK->ysize*pK->xsize; i++) {
      pMAC[i] = (char) rint(pK->Value.Value_val[i] * fConvWin[i]);
    }
    pMAC += pK->ysize*pK->xsize;
    }
  }
  return(TRUE);
}

/*
 * SAddServer() - return hostname (should free memory too).
 */
SAddServer(pRequest, pReply)
Packet *pRequest, *pReply;
{
  char sBuf[MAXSTR];
  gethostname(sBuf, MAXSTR);
  pReply->Packet_u.sOp = strdup(sBuf);
  pReply->op = SUCCESS;
}
