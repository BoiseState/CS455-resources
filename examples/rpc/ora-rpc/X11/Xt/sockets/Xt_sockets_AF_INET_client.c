#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xol/OpenLook.h>
#include <Xol/OblongButt.h>
#include <Xol/ControlAre.h>
#include <Xol/StaticText.h>

#define PORT        0x1234
#define DIRSIZE     8192

Arg             t_args[] = {
  {XtNstring, (XtArgVal) NULL},
  {XtNalignment, (XtArgVal) OL_LEFT},
};

Arg             b_args[] = {
  {XtNlabel, (XtArgVal) NULL},
};

extern int      errno;
Widget          w_top, w_text, w_control, w_but;	/* need widgets outside */
XtAppContext    thisApp;
XtInputId       input_id;	/* keep one global extra input source around */
int             sockd;		/* it will be watching one socket */
int             hosts;
int             host_num = 0;

void
SocketInputCallback(clientData, source, id)
  caddr_t         clientData;	/* notice we just use to check socket # */
  int            *source;	/* tells us what socket it is */
  XtInputId      *id;		/* tells us which input source */
{
  char            dir[DIRSIZE];
  int             fromlen;
  char          **argv = (char **) clientData;

  if (*id != input_id || *source != sockd)
    XtAppError(thisApp, "unexpected input from who knows where");
  printf("request on input source %d callback\n", *id);

  /*
   * See comments in Chapter 2 regarding read() and recvfrom(), we'll use the
   * more general latter choice here
   */
  if (recvfrom(*source, dir, DIRSIZE, 0, (struct sockaddr *) 0,
	       &fromlen) == -1) {
    perror("recvfrom");
    XtAppError(thisApp, "slave input socket read");
  } else {
    XtSetArg(t_args[0], XtNstring, (XtArgVal) dir);
    XtSetValues(w_text, t_args, XtNumber(t_args));
  }

  /*
   * Remove the callback input or things go berserk - the descriptor
   * always appears to have input pending.  We're done with it, so
   * throw it away.
   */
  XtRemoveInput(input_id);
  (void) close(*source);

  /*
   * Set up for next server and return
   */
  XtSetArg(b_args[0], XtNlabel, argv[(host_num++) % hosts + 2]);
  XtSetValues(w_but, b_args, XtNumber(b_args));
}

/*
 * Place an async remote request to a server
 */
void
ButtCallback(widget, clientData, callData)
  Widget          widget;
  caddr_t         clientData, callData;
{
  char          **argv = (char **) clientData;
  struct sockaddr_in sin;
  struct sockaddr_in pin;
  struct hostent *hp;

  /*
   * Go find out about the desired host machine
   */
  if ((hp = gethostbyname(argv[host_num % hosts + 2])) == 0) {
    perror("gethostbyname");
    XtAppError(thisApp, "gethostbyname");
  }
  /*
   * Fill in the socket structure with host information
   */
  memset(&pin, 0, sizeof(pin));
  pin.sin_family = AF_INET;
  pin.sin_addr.s_addr = ((struct in_addr *) (hp->h_addr))->s_addr;
  pin.sin_port = htons(PORT);

  /*
   * Grab an Internet domain socket, TCP transport.
   */
  if ((sockd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    XtAppError(thisApp, "socket");
  }
  /*
   * Connect to PORT on host.  We're not using /etc/services... or
   * any binding service here.
   */
  if (connect(sockd, &pin, sizeof(pin)) == -1) {
    perror("connect");
    XtAppError(thisApp, "connect");
  }
  /*
   * Send a message to the server PORT on machine HOST
   */
  if (send(sockd, argv[1], strlen(argv[1]), 0) == -1) {
    perror("send");
    XtAppError(thisApp, "send");
  }
  /*
   * Unblock the socket - don`t block when reading.
   */
  if (fcntl(sockd, F_SETFL, FNDELAY) == -1)
    XtAppError(thisApp, "unblocking socket");

  /*
   * Tell the widget to look at another source of events...
   */
  input_id = XtAppAddInput(thisApp,
			   sockd,
			   (caddr_t) XtInputReadMask,
			   SocketInputCallback,
			   (caddr_t) argv
    );
}

main(argc, argv)
  int             argc;
  char          **argv;
{

  w_top = OlInitialize(argv[0],
		       "HelloWorld",
		       NULL,
		       0,
		       &argc,
		       argv
    );


  /*
   * Create a control area and button to launch remote requests with.
   */
  w_control = XtCreateManagedWidget("control",
				    controlAreaWidgetClass,
				    w_top,
				    NULL,
				    0
    );
  hosts = argc - 2;
  host_num = 0;
  XtSetArg(b_args[0], XtNlabel, argv[host_num + 2]);
  w_but = XtCreateManagedWidget("button",
				oblongButtonWidgetClass,
				w_control,
				b_args,
				XtNumber(b_args)
    );
  XtAddCallback(w_but, XtNselect, ButtCallback, argv);

  /*
   * Create a text window to spew results into
   */
  XtSetArg(t_args[0], XtNstring, (XtArgVal) argv[0]);
  w_text = XtCreateManagedWidget("StaticText",
				 staticTextWidgetClass,
				 w_control,
				 t_args,
				 XtNumber(t_args)
    );

  XtRealizeWidget(w_top);
  thisApp = XtWidgetToApplicationContext(w_top);

  XtMainLoop();
}
