#include <stdio.h>
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

  if (*id != input_id || *source != xprt->xp_sock)
    XtAppError(thisApp, "unexpected input from who knows where");
  printf("request on input source %d callback\n", *id);

  readfds = svc_fdset;
  svc_getreqset(&readfds);	/* calls the dispatcher */
}
