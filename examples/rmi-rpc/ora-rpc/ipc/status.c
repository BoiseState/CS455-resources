#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>

static int      sigchild();
static int      visits = 0;

main()
{
  int             ppid, i;

  ppid = getpid();
  printf("parent process id: \"%d\"\n", ppid);
  signal(SIGCHLD, sigchild);	/* do sigchild() when child is killed */
  while (scanf("%d", &i) == 1) {
    ppid = fork();
    if (!ppid) {
      printf("in child, sleeping...\n");
      exit(i);
    }
  }

}

static int
sigchild()
{				/* lots of other interesting args passed to
				 * sigchild() */
  union wait      status;
  int             DeadPid;

  while ((DeadPid = wait(&status)) == -1);
  printf("found status \"%d\"\n", status.w_retcode);
  return;
}
