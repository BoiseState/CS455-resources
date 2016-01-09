/*
 * asyncRls.c: remote directory listing client
 */
#include <stdio.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <rpc/rpc.h>
#include "asyncRls.h"

#define SERVERFREE(argc, argv, h_name, k) for (k=1; k<argc; k+=2) \
	if (!strcmp(h_name, argv[k])) { k=(k-1)/2; break;} \
	if (k>=argc) {  \
	fprintf(stderr, "%s: where did you come from!\n", h_name); \
	exit(-1); }

char           *locald();
extern bool_t   xdr_dir();
char           *host[DIR_SIZE];

main(argc, argv)		/* Usage rls host dir host dir ... host dir */
  char          **argv;
  int             argc;
{
  extern int      errno;
  enum clnt_stat  clnt_stat;
  fd_set          readfds;
  struct timeval  timeout;
  int             i, j, k;
  int             ts = getdtablesize();	/* file descripptor table size */
  int             ServerBusy = 0;	/* outstanding requests bitmask */

  /*
   * Register a local UDP server daemon to collect results. We use
   * registerrpc() as we can afford to hide the server handle SVCXPRT *.
   * Data sent to this collection service includes the name of the
   * server asynchronously returning the results.  There are more
   * robust ways to retrieve the name of the calling party developed
   * elsewhere.  Note we really have no dispatcher here - just one
   * single procedure - locald().
   */


  registerrpc(DIRDPROG, DIRDVERS, LOCALD, locald, xdr_dir, xdr_void);

  /*
   * Clear the timeval to affect a poll when using select
   */
  timeout.tv_sec = timeout.tv_usec = 0;

  /* repeatedly call an async request to a server if registered */
  for (j = 0, i = 1; 1; i = (i + 2) % (argc - 1), j = (i - 1) >> 1) {
    /*
     * Send repeated requests to the servers specified.  BE CAREFUL here as
     * attempting to send a request to a machine that is blocked trying to
     * return results to locald will in turn block us!  There is another way
     * to fix this (use clnt_create timeouts), but that requires checking
     * with the client to see if it can answer (time consuming).  By creating
     * all the required client handles ahead of time, repeating the process
     * with callrpc() could be eliminated. Instead we keep-track locally.
     * Another alternative could have been async.  server child-processes.
     */
    if (!(ServerBusy & (0x1 << j))) {	/* server is free */
      clnt_stat = callrpc(argv[i], DIRPROG, DIRVERS, READDIR,
			  xdr_dir, argv[i + 1], xdr_void, 0);
      if (clnt_stat != 0)
	clnt_perrno(clnt_stat);
      else
	ServerBusy |= (0x1 << j);
    }
    /*
     * Look for a response from the other socket
     */
    readfds = svc_fdset;
    switch (select(ts, &readfds, (int *) 0, (int *) 0, &timeout)) {
    case -1:
      if (errno == EINTR)
	continue;
      perror("svc_run: - select failed");
      break;			/* leave beat server marked as busy */
    case 0:
      break;
    default:
      svc_getreqset(&readfds);
      /*
       * You never know who came back and in what order, you'll need some
       * kind of monitorig mechanism like this
       */
      SERVERFREE(argc, argv, host, k);
      printf("server %s, #%d returned!\n", host, k);
      ServerBusy ^= (0x1 << k);
    }
  }
}

char           *
locald(dir)			/* invoked if there is something at my
				 * socket... */
  char           *dir;		/* char dir[DIR_SIZE] */
{
  /*
   * As part of the protocol, we asked the server to send back it's host name
   * to expedite the monitoring process. It's rather involved to determine
   * autonomously from this side who it is.
   */
  sscanf(dir, "%s", host);

  /* spew-out the results and bail out of here! */
  printf("%s\n", dir);

  /* must send something back, so here's a dummy reply */
  return;
}
