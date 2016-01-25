#include <stdio.h>
#include <rpc/rpc.h>
#include "rls.h"

static void     dispatch();	/* the server program - does dispatching */
extern bool_t   xdr_dir();
struct timeval  tval;
static int      count = 0;
char		hostname[DIR_SIZE];

/*
 * Register the service, then wait for requests without consuming resources.
 */
main()
{
  SVCXPRT        *transp;

  /*
   * As we are registering a dispatch service, we
   * use svc_register() and make run-time procedure number validation 
   * it's task.  registerrpc() is used if registering each procedure
   * independently.
   */
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
  tval.tv_sec = tval.tv_usec = 0;

  /*
   * Now register servers of our won to use as ways to do child
   * multi-processing - one for each procedure the dispatcher will
   * receive requests for.
   */

  (void) gethostname(hostname, DIR_SIZE); /* children will use
			  hostname to callrpc() to ourselves */
  if (!fork()) {
    /*
     * Register a unique program for each child process, e.g.
     * increment CHLDDIRPROG when using registerrpc()
     */
    registerrpc(CHLDDIRPROG, CHLDDIRVERS, CHLDDIRPROC, 
	read_dir, xdr_dir, xdr_void);
  }
  
  /*
   * Start the parent and child servers
   */
  svc_run();
  (void) fprintf(stderr, "svc_run returned\n");
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
  enum clnt_stat  clnt_stat;
  CLIENT         *client;

  switch (rqstp->rq_proc) {
  case NULLPROC:
    (void) svc_sendreply(transp, xdr_void, 0);
    return;

  case READDIR:
    if (!svc_getargs(transp, xdr_dir, dir)) {
      svcerr_decode(transp);
      return;
    }
    count++;

    /*
     * Parent process makes an RPC to the same machine, causing a
     * child process to do the work.  Note the child must know who to
     * respond to, so we send the client handle to him/her.  In this
     * way the dispatcher is transparent, passing the request on to the
     * child
     */
    clnt_stat = callrpc(hostname, CHLDDIRPROG, CHLDDIRVERS,
    	CHLDDIRPROC
	xdr_dir, argv[i + 1], xdr_void, 0);
			       if (fork()) return;
    read_dir(dir);
    printf("%d\n", count);

    /*
     * Send the reply back.
     * Again, no need for clean-up - just die gracefully
     */
    if (!svc_sendreply(transp, xdr_dir, dir)) {
      svcerr_systemerr(transp);
      _exit(-1);
    }

    _exit(0);

    /*
     * put more procedures here...  case A: case Z:
     */


  default:
    svcerr_noproc(transp);
    return;
  }
}
