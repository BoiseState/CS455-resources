/*
 * rip_svc_proc.c: rip remote server procedure
 */
#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include "rip.h"
#include "rip_shared.h"

/*
 * rip_1() - server procedure to execute one of many rip operations.
 * As the protocol suggests, Request Packets are returned
 * with some error logging.
 */
Packet *rip_1(pRequest)
Packet *pRequest;
{
  /*
   * Allocate static space for the return data.
   */
  static Packet Reply;
  char sBuf[MAXSTR];

  /*
   * Free last result & check to see what operation is Requested.
   */
  Reply.op = ERROR;
  switch (pRequest->op) {
    case READIMAGE: /* send total image to each server */
      ReadImage(pRequest, &Reply); break;
    case READKERNEL: /* send kernel to each server */
      ReadKernel(pRequest, &Reply); break;
    case CONVOLVE: /* split & schedule up job across servers */
      Convolve(pRequest, &Reply); break;
    case ADDSERVER: /* locally add a server */
      AddServer(pRequest, &Reply); break;
    default: /* below is the kind of thing you should do in each function */
      gethostname(sBuf, MAXSTR);
      strcat(sBuf, ": don't understand operation");
      Reply.Packet_u.sOp = strdup(sBuf);
      Reply.op = SUCCESS;
  } 
  return (&Reply);
}
