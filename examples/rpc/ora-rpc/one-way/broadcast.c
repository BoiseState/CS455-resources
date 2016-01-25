#include <rpc/rpc.h>
#include <stdio.h>
#include <sys/socket.h>		/* we'll need sockets here */
#include <netdb.h>

/*
 * replyProc collects replies from the broadcast. Pipe the output through
 * sort(1) -u to get a unique listing of responding servers.
 */

static          bool_t
replyProc(res, who)
  void           *res;		/* Assume nothing comes back */
  struct sockaddr_in *who;	/* The address of who sent the reply */
{
  register struct hostent *hp;

  hp = gethostbyaddr((char *) &who->sin_addr, sizeof
		     who->sin_addr,
		     AF_INET);
  printf("%s %s\n", inet_ntoa(who->sin_addr),
	 (hp == NULL) ? "(unknown)" : hp->h_name);
  return (FALSE);
}

main(argc, argv)
  int             argc;
  char          **argv;
{
  enum clnt_stat  rpc_stat;
  u_long          prognum, versnum, procnum;

  if (argc != 4) {
    fprintf(stderr, "Usage: %s prognum versnum procnum\n",
	    argv[0]);
    exit(1);
  }
  prognum = (u_long) atoi(argv[1]);
  versnum = (u_long) atoi(argv[2]);
  procnum = (u_long) atoi(argv[3]);

  /*
   * See if anybody is out there.  Note procnum should expect no request
   * arguments and send nothing back or we'll have some en/decode problems,
   * potentially hosing the server.  NULLPROC does this.
   */
  rpc_stat = clnt_broadcast(prognum, versnum, procnum, xdr_void,
			 (char *) NULL, xdr_void, (char *) NULL, replyProc);
  if ((rpc_stat != RPC_SUCCESS) && (rpc_stat != RPC_TIMEDOUT)) {
    fprintf(stderr, "%s: broadcast failed: %s\n",
	    argv[0], clnt_sperrno(rpc_stat));
    exit(1);
  }
  exit(0);
}
