#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <rpc/rpc.h>
#include "asyncRls.h"

static void     dispatch();	/* the server program - does dispatching */
extern bool_t   xdr_dir();
struct timeval  tval;

/*
 * Register the service, then wait for requests without consuming resources.
 */
main()
{
  SVCXPRT        *transp;

  (void) pmap_unset(DIRPROG, DIRVERS);

  transp = svcudp_create(RPC_ANYSOCK);
  if (transp == NULL) {
    (void) fprintf(stderr, "cannot create udp service.\n");
    exit(1);
  }
  if (!svc_register(transp, DIRPROG, DIRVERS, dispatch, IPPROTO_UDP)) {
    (void) fprintf(stderr,
		   "unable to register (DIRPROG, DIRVERS, udp).\n");
    exit(1);
  }
  /*
   * If we wished to simultaneously register the service w/ TCP transport,
   * we'd do the following, but since we use callrpc() which uses UDP, we
   * won't bother.
   */
  /*
   * transp = svctcp_create(RPC_ANYSOCK, 0, 0); if (transp == NULL) {
   * (void)fprintf(stderr, "cannot create tcp service.\n"); exit(1); } if
   * (!svc_register(transp, DIRPROG, DIRVERS, dispatch, IPPROTO_TCP)) {
   * (void)fprintf(stderr, "unable to register (DIRPROG, DIRVERS, tcp).\n");
   * exit(1); }
   */
  /*
   * Set the time-out limit to 0 - makes the servers thrash a little while
   * attempting toi return results, but after repeated retries, they get
   * there.  tval=0 also reduces the chance that the callrpc() will get stuck
   * when I ^C the client, reducing the need to restart the servers.
   */
  tval.tv_sec = tval.tv_usec = 0;

  svc_run();
  (void) fprintf(stderr, "svc_run returned\n");
  exit(1);
}

/*
 * Decode the requested service and provide it.
 */
static void
dispatch(rqstp, transp)
  struct svc_req *rqstp;
  SVCXPRT        *transp;
{
  char            dir[DIR_SIZE];
  char            dhost[DIR_SIZE];
  struct hostent *host;
  struct sockaddr_in *sock_in;
  enum clnt_stat  clnt_stat;
  CLIENT         *client;
  int             sock = RPC_ANYSOCK;

  switch (rqstp->rq_proc) {
  case NULLPROC:
    (void) svc_sendreply(transp, xdr_void, 0);
    return;

  case READDIR:
    if (!svc_getargs(transp, xdr_dir, dir)) {
      svcerr_decode(transp);
      return;
    }
    if (!svc_sendreply(transp, xdr_void, 0)) {
      svcerr_systemerr(transp);
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
     * Return the result with a call to the requestor's local daemon - but
     * where does it live? Translate this into caller host info to call
     * client back
     */
    sock_in = svc_getcaller(transp);	/* get caller socket info */
    sock_in->sin_port = 0;	/* makes clntudp_create consult yp */
    client = clntudp_create(sock_in, DIRDPROG, DIRDVERS,
			    tval, &sock);
    clnt_stat = clnt_call(client, LOCALD, xdr_dir, dhost,
			  xdr_void, 0, tval);
    if (clnt_stat != 0)
      clnt_perrno(clnt_stat);

    /*
     * We'll reuse dhost, but should call this to free the XDR struct, plus
     * clean things up
     */
    svc_freeargs(transp, xdr_dir, dhost);
    clnt_destroy(client);
    return;

    /*
     * put more procedures here...  case A: case Z:
     */

  default:
    svcerr_noproc(transp);
    return;
  }
}
