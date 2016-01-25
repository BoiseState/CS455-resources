#include <rpc/rpc.h>
#include <sys/errno.h>

void svc_run()
{
  fd_set          readfdset;
  extern int      errno;
  static int      tsize = 0;
/*
 * Cache table size avoiding an expensive system call every time.
 */

  if (!tsize) tsize = getdtablesize();

  while (1) {
    readfdset = svc_fdset;
    switch (select(tsize, &readfdset, (fd_set*) NULL, (fd_set*) NULL,
                   (struct timeval *) NULL)) {
    case -1:
      if (errno == EBADF) continue;
      perror("select failed");
      return;
    case 0:
      /* perform other functions here if select() timed-out */
      continue;
    default:
      svc_getreqset(&readfdset);
    }
  }
}
