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
  static struct   itimerval timeout = {{1, 0}, {1, 0}};
  Notify_value    rpcread();

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

  notify_enable_rpc_svc(FALSE);
  notify_set_itimer_func(frame, rpcread, ITIMER_REAL, &timeout, NULL);

  xv_main_loop(frame);
}

read_dir(host, dir)
  char *dir, *host;
{
  extern bool_t   xdr_dir();
  enum clnt_stat  clnt_stat;

  clnt_stat = callrpc(host, DIRPROG, DIRVERS, READDIR,
		      xdr_dir, dir, xdr_dir, dir);
  if (clnt_stat != 0) clnt_perrno(clnt_stat);
}

Notify_value rpcread(item, event)
  Panel_item      item;
  Event          *event;
{
  i = (i + 1) % hosts;
  textsw_insert(textsw, hostnames[i], strlen(hostnames[i]));
  read_dir(hostnames[i], dir);	/* read_dir(host, directory) */
  textsw_insert(textsw, dir, strlen(dir));
  return (NOTIFY_DONE);
}
