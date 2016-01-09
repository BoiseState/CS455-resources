#include <stdio.h>
#include <rpc/rpc.h>
#include "rtele.h"

main(argc, argv)
  int             argc;
  char           *argv[];
{
  CLIENT         *cl;		/* a client handle, same as it ever was */

  if (argc != 3) {
    fprintf(stderr, "Usage: %s server_host name\n", argv[0]);
    exit(1);
  }
   /*            
    * Create client "handle" used for calling DIRPROG on the
    * server designated on the command line. We tell the rpc package
    * to use the "tcp" protocol when contacting the server.
    */
   /*cl = clnt_create(argv[1], RTELEPROG, RTELEVERS, "visible");*/
   /*cl = clnt_create(argv[1], RTELEPROG, RTELEVERS, "circuit_n");*/
   /*cl = clnt_create(argv[1], RTELEPROG, RTELEVERS, "tcp");*/
   cl = clnt_create(argv[1], RTELEPROG, RTELEVERS, "netpath");
   if (cl == NULL) {
      /*
       * Couldn't establish connection with server.
       * Print error message and die.
       */
      clnt_pcreateerror(argv[1]);
      exit(1);
   }  
      
   /*
    * Call the remote procedure "readdir" on the server and print
    * results.
    */
  fputs("name\tlocation\textension\n", stdout);
  fputs(*(rtele_1(&argv[2], cl)), stdout);
  putchar('\n');
}
