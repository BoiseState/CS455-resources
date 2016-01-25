#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <errno.h>

#define MAXCHLD 100		/* Will sure get stopped by process limit */

int             ChildStart[MAXCHLD];	/* When the child got started */
int             ChildTime[MAXCHLD];	/* Duration of the child process */
int             ChildPid[MAXCHLD];	/* Child process ID */
struct timeval  tval;

extern int      errno;
static int      sigchild();

main()
{
  int             i, child;


  /* install SIGCHLD catcher */
  signal(SIGCHLD, sigchild);

  /* fork up to MAXCHLD children, and let them report back */
  while (scanf("%d", &i) == 1)
    for (child = 0; child < MAXCHLD; child++) {
      gettimeofday(&tval, NULL);
      ChildStart[child] = tval.tv_sec;
      if (ChildPid[child] = fork()) {
	/* in parent */
	if (ChildPid[child] == -1)
	  perror("can't fork another child");
	if (EAGAIN == errno) {
	  printf("ran out of processes after %d th child\n", child);
	  errno = 0;
	  break;
	}
      } else {
	/* in child */
	sleep(i);
	_exit(child);
      }
    }
}

static int
sigchild()
{				/* lots of other interesting args passed to
				 * sigchild() */
  union wait      status;
  int             DeadPid;
  int             i;

  gettimeofday(&tval, NULL);
  DeadPid = wait(&status);
  for (i = 0; i < MAXCHLD; i++) {
    if (DeadPid == ChildPid[i]) {
      ChildTime[i] = tval.tv_sec - ChildStart[i];
      printf("found status \"%d\", took %d seconds\n",
	     status.w_retcode, ChildTime[i]);
      return;
    }
  }
  printf("pid %d: bastard child\n", DeadPid);
}
