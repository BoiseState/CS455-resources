/*
 * rls.c: remote directory listing client
 */
#include <stdio.h>
#include <strings.h>
#include <rpc/rpc.h>
#include "rls.h"

main(argc, argv)
  int             argc;
  char           *argv[];
{
  char            d[DIR_SIZE];

  /* call the remote procedure if registered */
  strcpy(d, argv[2]);
  read_dir(argv[1], d);	/* read_dir(host, directory) */

  /* spew-out the results and bail out of here! */
  printf("%s\n", d);

  exit(0);
}

read_dir(host, d)
  char           *d, *host;
{
  extern bool_t   xdr_dir();
  enum clnt_stat  clnt_stat;
  static struct timeval TIMEOUT = {25, 0};
  CLIENT         *clnt;

  clnt = clnt_create(host, DIRPROG, DIRVERS, "tcp");
  clnt_stat = clnt_call(clnt, READDIR,
			xdr_dir, d, xdr_dir, d, TIMEOUT);
  if (clnt_stat != 0)
    clnt_perrno(clnt_stat);
  clnt_destroy(clnt);
}
