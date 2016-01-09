#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <rpc/rpc.h>
#include "../async/asyncRls.h" /* header file generated for the FRPC clients */

static void     dispatch();     /* the server program - does dispatching */
extern bool_t   xdr_dir();
struct timeval  tval;
static int      count = 0;

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
    count++;
    if (fork()) return;      /* parent goes back to listening for requests */
    read_dir(dir);
    /*
     * Pre-pend the host name to ease client-side tracking
     */
    (void) gethostname(dhost, DIR_SIZE);
    strcat(dhost, "\n");
    strcat(dhost, dir);
    printf("%d\n", count);

    /*
     * Return the result with a call to the requestor's local daemon - but
     * where does it live? Translate this into caller host info to call
     * client back
     */
    sock_in = svc_getcaller(transp);    /* get caller socket info */
    sock_in->sin_port = 0;      /* makes clntudp_create consult yp */
    client = clntudp_create(sock_in, DIRDPROG, DIRDVERS,
                            tval, &sock);
    clnt_stat = clnt_call(client, LOCALD, xdr_dir, dhost,
                          xdr_void, 0, tval);
    if (clnt_stat != 0)
      clnt_perrno(clnt_stat);

    /*
     * We're about to die, manual clean-up is unnecessary.
     */
    _exit(0);

    /*
     * put more procedures here...  case A: case Z:
     */

  default:
    svcerr_noproc(transp);
    return;
  }
}
