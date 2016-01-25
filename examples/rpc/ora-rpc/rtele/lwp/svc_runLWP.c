#include <rpc/rpc.h>
#include <sys/errno.h>
#include <lwp/lwp.h>
#define MAXSVC 50
#define MAXPRIO 10
void lwp_perror() { extern int errno; perror(errno); }

void
svc_run()
{
  extern int      errno;
  fd_set          readfds;
  int             size = getdtablesize();
  thread_t        tid;

  lwp_setstkcache(1024, MAXSVC + 1);	/* go thread yourself */

  while (1) {
    readfds = svc_fdset;

    /* 
     * Block if they're no new requests, -lbnio yields the thread.  If
libnbio.a
     * is not available, try select(timeval != 0) polling.
     */

    switch (select(size, &readfds, (int *) NULL, (int *) NULL, NULL)) {
    case -1:
      if (errno == EINTR)
	continue;
      perror("select failed");
      return;
    case 0:
		printf("polling...");
      continue;
    default:
		/* discourage false activity */
      if (!bcmp(readfds, svc_fdset, sizeof(fd_set))) continue;
      puts("servicing...");
      lwp_create(&tid, svc_getreqset, MINPRIO, 0, lwp_newstk(), 1, readfds);
      /*
       * Main thread - wait here until the new thread has removed the pending
       * input as found by select(), effectively starting the request
       * servicing.  If we had multiple requests at once this logic steps
       * thru them one at a time, not letting the parent go on until the last
       * request packet is read...
       * 
       * A simple way to do this is to give the new thread priority until the
       * request is started.  The service procedure needs to yield at some
       * point or svc_run() never continues.
       */
      lwp_setpri(SELF, MINPRIO);
      lwp_yield(tid);  /* returns when new thread is blocked */
      lwp_setpri(SELF, MAXPRIO);
      puts("back to selecting...");
    }
  }
}
