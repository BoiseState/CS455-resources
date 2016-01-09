#include <xview/xview.h>
#include <xview/textsw.h>
#include <xview/panel.h>
#include <rpc/rpc.h>
#include "rls.h"

int             hosts;
char          **hostnames;
char            dir[DIR_SIZE];
static int      i = 0;
Textsw          textsw;
Frame           frame;

main(argc, argv)
  int             argc;
  char           *argv[];
{
  Xv_window       window;
  void            event_proc();

  /*
   * Initialize XView and get host names and directory
   */
  xv_init(XV_INIT_ARGS, argc, argv, 0);
  hosts = argc - 2;
  strcpy(dir, argv[1]);
  hostnames = &argv[2];

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
	 WIN_CONSUME_EVENTS, WIN_UP_EVENTS, NULL,
	 NULL);

  xv_main_loop(frame);
}

void
event_proc(window, event)
  Xv_Window       window;
  Event          *event;
{
  if (event_action(event) == ACTION_SELECT) {
    xv_set(frame, FRAME_BUSY, TRUE, NULL);
    i = (i + 1) % hosts;
    textsw_insert(textsw, hostnames[i], strlen(hostnames[i]));
    /*
     * This will block, so one could spawn children or threads as an
     * alternative. Extra signals etc. REALLY complicate things...
     */
    read_dir(hostnames[i], dir);
    textsw_insert(textsw, dir, strlen(dir));
    xv_set(frame, FRAME_BUSY, FALSE, NULL);
  }
}

read_dir(host, dir)
  char           *dir, *host;
{
  extern bool_t   xdr_dir();
  enum clnt_stat  clnt_stat;

  clnt_stat = callrpc(host, DIRPROG, DIRVERS, READDIR,
		      xdr_dir, dir, xdr_dir, dir);
  if (clnt_stat != 0) clnt_perrno(clnt_stat);
}
