#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <errno.h>

#define MAXCHLD 1000		/* Will sure get stopped by process limit */

int             ChildStart[MAXCHLD];	/* When the child got started */
int             ChildTime[MAXCHLD];	/* Duration of the child process */
int             ChildPid[MAXCHLD];	/* Child process ID */
struct itimerval tval;

extern int      errno;
static int      sigchild();

main()
{
  int             i, child;

  /* Set the real-time timer */
  timerclear(&tval.it_interval);
  timerclear(&tval.it_value);
  tval.it_value.tv_sec =	/* don't leave it zero! */
    tval.it_interval.tv_sec = 100;	/* seconds */
  setitimer(ITIMER_REAL, &tval, NULL);

  /* install SIGCHLD catcher */
  signal(SIGCHLD, sigchild);

  /* fork up to MAXCHLD children, and let them report back */
  while (scanf("%d", &i) == 1)
    for (child = 0; child < MAXCHLD; child++) {
      getitimer(ITIMER_REAL, &tval);
      ChildStart[child] = tval.it_value.tv_sec;
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

  getitimer(ITIMER_REAL, &tval);
  DeadPid = wait(&status);
  for (i = 0; i < MAXCHLD; i++) {
    if (DeadPid == ChildPid[i]) {
      ChildTime[i] = ChildStart[i] - tval.it_value.tv_sec;
      printf("found status \"%d\", took %d seconds\n",
	     status.w_retcode, ChildTime[i]);
      return;
    }
  }
  printf("pid %d: bastard child\n", DeadPid);
}
