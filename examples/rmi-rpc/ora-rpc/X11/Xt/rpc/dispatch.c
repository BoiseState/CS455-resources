#include <stdio.h>
#include <rpc/rpc.h>
#include "rls.h"

/*
 * Decode the requested service and provide it. This is a synchronous
 * dispatcher, but there's no reason why you couldn't use the async.
 * rls_svc.c dispatcher to avoid prolonged RPC client blocking.
 */
void
dispatch(rqstp, transp)
  struct svc_req *rqstp;
  SVCXPRT        *transp;
{
  extern bool_t   xdr_dir();
  extern char    *read_dir();
  char            dir[DIR_SIZE];
  char            dhost[DIR_SIZE];

  switch (rqstp->rq_proc) {
  case NULLPROC:
    (void) svc_sendreply(transp, xdr_void, 0);
    return;

  case READDIR:
    if (!svc_getargs(transp, xdr_dir, dir)) {
      svcerr_decode(transp);
      return;
    }
    read_dir(dir);
    /*
     * Pre-pend the host name to ease client-side tracking
     */
    (void) gethostname(dhost, DIR_SIZE);
    strcat(dhost, "\n");
    strcat(dhost, dir);

    /*
     * Return the result 
     */
    if (!svc_sendreply(transp, xdr_dir, dir)) {
      svcerr_systemerr(transp);
      return;
    }
    /*
     * We'll reuse dhost, but should call this to free the XDR struct, plus
     * clean things up
     */
    svc_freeargs(transp, xdr_dir, dhost);
    return;

    /*
     * put more procedures here...  case A: case Z:
     */

  default:
    svcerr_noproc(transp);
    return;
  }
}
