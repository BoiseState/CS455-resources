/*
 * one-way_svc_pro.c: remote namedir_1() and readdir_1() implementations for
 * the multiple directory listing service.
 */
#include <rpc/rpc.h>
#include <sys/dir.h>
#include "one-way.h"

extern int      errno;
extern char    *malloc();
extern char    *strdup();

static readdir_res res;		/* must be static! */
static int      beginCycleFlag = TRUE;

void           *
dirname_1(dirname)
  nametype       *dirname;
{
  namelist        nl;
  namelist       *nlp;
  DIR            *dirp;
  struct direct  *d;

  /*
   * If this is the first time a no-reply request has been made in this
   * cycle, start a new res.readdir_res_u.list. Free previous result.  It
   * might be a long list that eats-up memory.
   */
  if (beginCycleFlag == TRUE) {
    beginCycleFlag = FALSE;
    xdr_free(xdr_readdir_res, &res);
    res.readdir_res_u.list = NULL;
  }
  /*
   * Open the directory.
   */
  dirp = opendir(*dirname);
  if (dirp == NULL) {
    res.errno = errno;
    return (NULL);
  }
  /*
   * Add directory entries to the end of the list.
   */
  nlp = &res.readdir_res_u.list;
  while ((nl = *nlp) != NULL)
    nlp = &nl->pNext;

  while (d = readdir(dirp)) {
    nl = *nlp = (namenode *) malloc(sizeof(namenode));
    nl->name = strdup(d->d_name);
    nlp = &nl->pNext;
  }
  *nlp = NULL;

  /*
   * Return no result.
   */
  res.errno = 0;
  closedir(dirp);
  return (NULL);
}

readdir_res    *
readdir_1(dirname)
  nametype       *dirname;
{

  /*
   * Record this directory's contents.
   */
  dirname_1(dirname);

  /*
   * Start the cycle over and return the result.
   */
  beginCycleFlag = TRUE;
  return (&res);
}
