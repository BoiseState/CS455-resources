#include <stdio.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <Xol/OpenLook.h>
#include <Xol/ControlAre.h>
#include <rpc/rpc.h>
#include "rls.h"

XtInputId       input_id;
SVCXPRT        *xprt;
XtAppContext    thisApp;
struct timeval  timeout;
int             ts;

main(argc, argv)
  int             argc;
  char           *argv[];
{
  Widget          w_top, w_control;
  extern bool_t   xdr_dir();
  extern char    *read_dir();
  extern void     dispatch();	/* the RPC server program - does dispatching */
  static void     SocketInputCallback();	/* event dispatch routine */

  /*
   * Establish the server daemon
   */
  (void) pmap_unset(DIRPROG, DIRVERS);
  xprt = svctcp_create(RPC_ANYSOCK, 0, 0);
  if (xprt == NULL) {
    (void) fprintf(stderr, "cannot create tcp service.\n");
    exit(1);
  }
  if (!svc_register(xprt, DIRPROG, DIRVERS, dispatch, IPPROTO_TCP)) {
    (void) fprintf(stderr,
		   "unable to register (DIRPROG, DIRVERS, tcp).\n");
    exit(1);
  }
  timeout.tv_sec = timeout.tv_usec = 1; /* give select time... */
  ts = getdtablesize();		/* file descripptor table size */


  /*
   * Build-up a minimal widget - though we won't be interacting w/ the
   * windows, we must manage some widgets.
   */
  w_top = OlInitialize(argv[0],
		       "rls server",
		       NULL,
		       0,
		       &argc,
		       argv
    );
  w_control = XtCreateManagedWidget("control",
				    controlAreaWidgetClass,
				    w_top,
				    NULL,
				    0
    );

  XtRealizeWidget(w_top);
  thisApp = XtWidgetToApplicationContext(w_top);

  /*
   * Before starting the dispatch loop, tell it to look at the server socket
   * information in fd_set to notify us of RPC requests.
   */
  input_id = XtAppAddInput(thisApp,
			   xprt->xp_sock,
			   (caddr_t) XtInputReadMask,
			   SocketInputCallback,
			   (caddr_t) NULL
    );

  /* loops continuously */
  XtMainLoop();
}

static void
SocketInputCallback(clientData, source, id)
  caddr_t         clientData;
  int            *source;
  XtInputId      *id;
{
  fd_set          readfds;
  extern int      errno;

  if (*id != input_id || *source != xprt->xp_sock)
    XtAppError(thisApp, "unexpected input from who knows where");
  printf("request on input source %d callback\n", *id);

  readfds = svc_fdset;
  switch (select(ts, &readfds, (int *) 0, (int *) 0, &timeout)) {
  case -1:
    if (errno == EINTR)
      break;
    perror("my svc_run: - select failed");
    break;			/* leave beat server marked as busy */
  case 0:
    break;
  default:
    /*
     * Calls the dispatcher - first time it's the in-active TCP descriptor.
     * Check for the active TCP descriptors that show-up in svc_fdset.
     */
    while (select(ts, &readfds, (int *) 0, (int *) 0, &timeout) != 0) {
      svc_getreqset(&readfds);	/* calls the dispatcher */
      readfds = svc_fdset;
    }
  }
}
