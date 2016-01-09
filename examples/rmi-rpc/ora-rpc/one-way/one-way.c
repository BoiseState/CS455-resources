/*
 * one-way.c: one-way remote multiple directory listing client
 */
#include <stdio.h>
#include <rpc/rpc.h>
#include "one-way.h"

extern int      errno;
struct timeval  TIMEOUT = {0, 0};	/* used by one-way_clnt.c with clnt_call() timeouts */

main(argc, argv)
  int             argc;
  char           *argv[];
{
  CLIENT         *cl;
  char           *server;
  char           *dir;
  readdir_res    *result;
  namelist        nl;
  int             i;

  if (argc < 3) {
    fprintf(stderr, "Usage: %s host directory(s)\n", argv[0]);
    exit(1);
  }
  server = argv[1];

  /*
   * Create client "handle" used for calling the procedures of the program.
   * We use the "tcp" prtocol to assure our one-way requests get there.
   */
  if ((cl = clnt_create(server, DIRPROG, DIRVERS, "tcp")) == NULL) {
    clnt_pcreateerror(server);
    exit(1);
  }
  /*
   * Call the remote procedure DIRNAME on the server each time for all but
   * the last directory specified on the command line.  Set the time-out for
   * this client handle to zero to tell RPC not to wait for a reply.
   */
  TIMEOUT.tv_sec = TIMEOUT.tv_usec = 0;
  if (clnt_control(cl, CLSET_TIMEOUT, &TIMEOUT) == FALSE) {
	 fputs("can't zero timeout\n", stderr);
	 exit(1);
  }
  for (i = 2; i < argc - 1; i++)  {
    dirname_1(&(argv[i]), cl);
    clnt_perror(cl, server);	/* ignore the time-out errors */
  }

  /*
   * Now send a normal sync. RPC to signal the server to send the reply.
   * First you must set the time-out back to some realistic non-zero value.
   */
  TIMEOUT.tv_sec = 25;
  if (clnt_control(cl, CLSET_TIMEOUT, &TIMEOUT) == FALSE) {
	 fputs("can't delay timeout\n", stderr);
	 exit(1);
  }
  if ((result = readdir_1(&(argv[i]), cl)) == NULL) {
    clnt_perror(cl, server);
    exit(1);
  }
  /*
   * Successfully called the remote procedures.
   */

  if (result->errno != 0) {
    /*
     * a remote system error occurred, print error message and die
     */
    errno = result->errno;
    perror(dir);
    exit(1);
  }
  /*
   * Got a directory listing, print it out
   */
  for (nl = result->readdir_res_u.list; nl != NULL; nl = nl->pNext) {
    printf("%s\n", nl->name);
  }
  exit(0);
}
