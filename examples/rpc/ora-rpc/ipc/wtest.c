#include <stdio.h>
#include <sys/wait.h>

int             ServerStatus[10];

main()
{
  int             pid, ppid, DeadPid;
  int             i, j;

  ppid = getpid();
  for (i = 0; i < 10; i++) {
    while ((ServerStatus[i] = fork()) == -1) {
      sleep(1);
    }

    if ((pid = getpid()) != ppid) {	/* child */
      printf("child %d exits...\n", pid);
      exit(0);
    } else {			/* parent */
      DeadPid = wait(0);
      for (j = 0; j < 10; j++) {
	if (ServerStatus[j] == DeadPid) {
	  printf("parent caught child %d\n", DeadPid);
	  break;
	}
      }
    }
  }
}
