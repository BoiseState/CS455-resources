#include <stdio.h>
#include <sgtty.h>
#include <lwp/lwp.h>
/*#include <lwp/stackdep.h>*/
#include <rpc/rpc.h>
#include "rtele.h"
#define MAXPRIO 10
#define MAXCLNT 10
#define RAW_ON(fd) { \
  struct sgttyb ttyb; \
  gtty(fd, &ttyb) ;  /* get current terminal characteristics */ \
  ttyb.sg_flags |= RAW ; \
  stty (fd, &ttyb) ; /* set RAW on */ \
  }
#define RAW_OFF(fd) { \
  struct sgttyb ttyb; \
  gtty(fd, &ttyb) ;  /* get current terminal characteristics */ \
  ttyb.sg_flags ^= RAW ; \
  stty (fd, &ttyb) ; /* set RAW off */ \
  }

void            lwp_perror() {
  printf("help\n");
  fflush(stdout);
};

/* need to keep these in all threads (inc. main) */
CLIENT         *cl[MAXCLNT];
char           *server[MAXCLNT];
char           *user[MAXCLNT];
int             numServers = 0;
FILE           *fp;

main(argc, argv)
  int             argc;
  char          **argv;
{
  char            c;
  thread_t        tid;
  int             request();
  int             i;

  /*
   * Format of argv is "server username" pairs.  Open a client handle for
   * each server.
   */
  if ((argc < 3) || ((argc-1)/2 > MAXCLNT)) {
    (void) fprintf(stderr, 
		"Usage: %s server username [server username [[[]]]]\n", argv[0]);
    (void) fprintf(stderr, "\t\tup to %d pairs\n", MAXCLNT);

    exit(1);
  }
  fp = fopen("/dev/console", "w");	/* give the threads somewhere to
					 * write */

  for (i = 1; i < argc; i += 2) {
    cl[i >> 1] = clnt_create(argv[i], RTELEPROG, RTELEVERS, "tcp");
    server[i >> 1] = (char *) strdup(argv[i]);
    user[i >> 1] = (char *) strdup(argv[i + 1]);
    numServers++;
  }

  /*
   * Establish an upper limit on priority for the whole pod, 1..10.  Main
   * becomes a LWP running at 10.
   */
  (void) pod_setmaxpri(MAXPRIO);
  (void) puts("lwp main here");

  /*
   * Initialize a cache of stacks (at least 1K bytes).  Let's assume we'll
   * never have more than MAXCLNT RPC requests out at once.
   */
  lwp_setstkcache(1024, MAXCLNT + 1);

  /*
   * For each "server username" pair in argv, launch an RPC in a new thread.
   * Main will block when waiting for input, giving control to request().
   * Any char will terminate.
   */

  RAW_ON(0);			/* enables raw I/O at stdin/out */
  while (read(0, &c, 1) && (c == '.')) {
    for (i = 0; i < numServers; i++) {
      /*
       * Start another lower-priority thread to follow-through w/ a blocking
       * RPC. Establish a stack for the thread that indicates which client
       * request we're interested in.  This must be private to keep others
       * from stepping on it.
       */
      (void) fprintf(fp, "start: %dth request thread, server %s username %s\n",
		     i, server[i], user[i]);
      lwp_create((thread_t *) 0, request, MINPRIO, 0, lwp_newstk(), 1, i);
    }
  }
  RAW_OFF(0);
  exit(1);
}

request(serve)
  int             serve;
{
  /*
   * The RPC will bock this thread for a while. Execution falls-thru and
   * terminates this thread.  Could use lwp_destroy(lwp_self())).
   */
  fprintf(fp, "return: %dth request thread, server %s username %s\n%s",
      serve, server[serve], user[serve], *rtele_1(&user[serve], cl[serve]));
}
