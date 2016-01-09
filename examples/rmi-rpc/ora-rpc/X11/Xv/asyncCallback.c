#include <xview/xview.h>
#include <xview/textsw.h>
#include <xview/panel.h>
#include <sys/errno.h>
#include <rpc/rpc.h>
#include "../../rls/manual/async/asyncRls.h"

int             hosts;
char          **hostnames;
char            dir[DIR_SIZE];
static int      i = 0;
Textsw          textsw;
Frame           frame;
int             ts;	/* file descriptor table size */
struct timeval  stimeout; /* select time out values */
extern int      errno;

main(argc, argv)
  int             argc;
  char           *argv[];
{
  Xv_window       window;
  void            event_proc();
  static struct   itimerval timeout = {{1, 0}, {1, 0}};
  Notify_value    getresults();
  char           *locald();
  extern bool_t   xdr_dir();

  /*
   * Initialize XView and get host names and directory
   */
  xv_init(XV_INIT_ARGS, argc, argv, 0);
  hosts = argc - 2;
  strcpy(dir, argv[1]);
  hostnames = &argv[2];

  /*
   * Establish some one-time use stuff for using select repeatedly
   */
  ts = getdtablesize();
  stimeout.tv_sec = stimeout.tv_usec = 0;

  /*
   * Create windows -- base frame and text subwindow.
   */
  frame = xv_create(XV_NULL, FRAME, FRAME_LABEL, argv[0], NULL);
  textsw = xv_create(frame, TEXTSW,
		     WIN_ROWS, 20,
		     WIN_COLUMNS, 80,
		     NULL);
  window_fit(frame);

  /*
   * ...and tell it what events to honor and how
   */
  window = (Xv_window) xv_get(textsw, OPENWIN_NTH_VIEW, 0);
  xv_set(window,
	 WIN_EVENT_PROC, event_proc,
	 WIN_IGNORE_EVENTS, WIN_UP_EVENTS, NULL,
	 NULL);

  /*
   * Register a collection daemon as a timer routine then start looping
   */
  registerrpc(DIRDPROG, DIRDVERS, LOCALD, locald, xdr_dir, xdr_void);
  notify_set_itimer_func(frame, getresults, ITIMER_REAL,
                         &timeout, NULL);
  xv_main_loop(frame);
}

void event_proc(window, event)
  Xv_Window       window;
  Event          *event;
{
  if (event_action(event) == ACTION_SELECT) {
    printf("sending request\n");
    i = (i + 1) % hosts;
    /*
     * Issue an async. rpc giving no real answer, proceeding
     * back to the notifier loop.  getresults() is caller
     * by timer notices to check for returns.
     */
    read_dir(hostnames[i], dir);
  }
}

read_dir(host, dir)
  char           *dir, *host;
{
  extern bool_t   xdr_dir();
  enum clnt_stat  clnt_stat;

  /*
   * Place the call.  Notice the return value is not inspected.
   */
  clnt_stat = callrpc(host, DIRPROG, DIRVERS, READDIR,
		      xdr_dir, dir, xdr_void, 0);
  if (clnt_stat != 0) clnt_perrno(clnt_stat);
}

Notify_value getresults(item, event)
  Panel_item      item;
  Event          *event;
{
  fd_set          readfds;

  /*
   * Look for a response from the other socket, return otherwise.
   */
  readfds = svc_fdset;
  switch (select(ts, &readfds, (int *) 0, (int *) 0, &stimeout)) {
    case -1:
      if (errno == EINTR) break;
      perror("svc_run: - select failed");
      break;
    case 0:
      break;
    default:
      svc_getreqset(&readfds); /* calls locald() */
  }
  return (NOTIFY_DONE);
}

char *locald(dir) /* invoked if there is something at my socket... */
  char           *dir;          /* char dir[DIR_SIZE] */
{
  /*
   * As part of the protocol, we asked the server to send back it's
   * host name to expedite any required monitoring process. 
   */
  textsw_insert(textsw, dir, strlen(dir));
  return;
}
