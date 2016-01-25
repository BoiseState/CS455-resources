#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include "rtele.h"

char          **
rtele_1(name)
  char          **name;
{
  char            sb[MAX_STR];
  static char     sa[MAX_STR];
  static char    *s = sa;
  FILE           *fp = fopen(DATABASE, "r");

  sa[0] = NULL;
  while (fgets(sa, MAX_STR - 1, fp)) {
    sscanf(sa, "%s%s%s", sb,sb,sb); /* get the last name */
    if (!strcmp(sb, *name)) break;
  }
  if feof(fp) sa[0] = NULL; /* no match */
  fclose(fp);
  return ((char **)&s);
}
