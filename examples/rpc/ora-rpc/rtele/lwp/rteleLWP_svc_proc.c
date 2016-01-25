#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include <lwp/lwp.h>
#include "rtele.h"

char          **
rtele_1(name, rqstp)
  char          **name;
  struct svc_req *rqstp;
{
  char            sb[MAX_STR];
  static char     sa[MAX_STR];
  static char    *s = sa;
  FILE           *fp = fopen(DATABASE, "r");
  static struct timeval tv = {1, 0};

  /*
   * After announcing ourselves, give priority back to the dispatcher and
   * svc_run() to look for other requests.  Processing continues here only
   * when the main thread and older sibling threads of the same priority get
   * blocked (-lbnio) or yield, resp.
   */
  printf("thread %d started\t", SELF); fflush(stdout);
  lwp_resched(MINPRIO);    /* move yourself to the back of the queue if
                              there is one */
  lwp_sleep(&tv);    /* pretend service takes a long time, 
                        possible blocking.  This is where main gets
                        time to do selects */

  lwp_setpri(SELF, MINPRIO+1);    /* get a hold of all variables etc. */
  sa[0] = NULL;
  while (fgets(sa, MAX_STR - 1, fp)) {
    sscanf(sa, "%s%s%s", sb, sb, sb);  /* get the last name */
    if (!strcmp(sb, *name)) break;
  }
  if feof (fp) sa[0] = NULL;    /* no match */
  fclose(fp);

  printf("thread %d complete\t", SELF); fflush(stdout);
  return ((char **) &s);
}
