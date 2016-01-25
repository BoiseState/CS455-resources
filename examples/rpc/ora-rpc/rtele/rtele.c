#include <stdio.h>
#include <rpc/rpc.h>
#include "rtele.h"

main(argc, argv)
  int             argc;
  char           *argv[];
{
  CLIENT         *cl;		/* a client handle, discussed later */

  if (argc != 3) {
    fprintf(stderr, "Usage: %s server_host name\n", argv[0]);
    exit(1);
  }
  cl = clnt_create(argv[1], RTELEPROG, RTELEVERS, "tcp");

  fputs("name\t\textension\t\tlocation\n", stdout);
  fputs(*(rtele_1(&argv[2], cl)), stdout);
  putchar('\n');
}
