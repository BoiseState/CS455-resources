#include <stdio.h>
#include <xview/xview.h>
#include <xview/notify.h>
#include <sys/types.h>
#include <rpc/rpc.h>
#include "rls.h"

struct timeval  tval;

main(argc, argv)
  int             argc;
  char           *argv[];
{
  SVCXPRT        *xprt;
  extern void dispatch();/* the server program - does dispatching */

  (void) pmap_unset(DIRPROG, DIRVERS);

  /*
   * Establish the server daemon
   */
  xprt = svcudp_create(RPC_ANYSOCK);
  if (xprt == NULL) {
    (void) fprintf(stderr, "cannot create udp service.\n");
    exit(1);
  }
  if (!svc_register(xprt, DIRPROG, DIRVERS, dispatch, IPPROTO_UDP)) {
    (void) fprintf(stderr,
		   "unable to register (DIRPROG, DIRVERS, udp).\n");
    exit(1);
  }
  tval.tv_sec = tval.tv_usec = 10;

  /*
   * Before starting the notifier loop, tell it to look at the server socket
   * information in fd_set to notify us of RPC requests.
   */
  notify_enable_rpc_svc(TRUE);

  /* loops continuously */
  notify_start();
}
