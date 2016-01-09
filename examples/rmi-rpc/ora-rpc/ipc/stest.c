#include <signal.h>
#include <stdio.h>

void      sigchild();
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
      sleep(2);
      printf("...but child died on its own...\n");
      exit();
    } else {
      sleep(1);
      printf("parent signalling child with %d...\n", i);
      kill(ppid, i);
    }
  }

}

void
sigchild(sig)	/* 19 and 20 will zap things before normal death */
int sig;
{				/* lots of other interesting args passed to
				 * sigchild() */
  visits++;
  printf("on the %dth signal\n", visits);
  printf("found signal \"%d\"\n", sig);
  return;
}
