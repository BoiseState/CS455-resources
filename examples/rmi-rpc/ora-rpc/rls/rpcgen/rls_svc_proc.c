/*
 * rls_svc_pro.c: remote readdir_1() implementation
 */
#include <rpc/rpc.h>
#include <sys/dir.h>
#include "rls.h"

extern int errno;
extern char *malloc();
extern char *strdup();

readdir_res *
readdir_1(dirname)
	nametype *dirname;
{
	namelist nl;
	namelist *nlp;
	static readdir_res res;	/* must be static! */
	static DIR *dirp = NULL;/* static to see if we've been called before */
	struct direct *d;

	/*
	 * Open the directory.
	 */
	dirp = opendir(*dirname);
	if (dirp == NULL) {
		res.errno = errno;
		return(&res);
	}

	/* 
	 * Free previous result if there was one.  It might be a long list
	 * that eats-up memory.
	 */
	if (dirp) xdr_free(xdr_readdir_res, &res);

	/*
	 * Collect directory entries.  Memory allocated here will
	 * be xdr_free'd next time around.
	 */
	nlp = &res.readdir_res_u.list;
	while (d = readdir(dirp)) {
		nl = *nlp = (namenode *)malloc(sizeof(namenode));
		nl->name = strdup(d->d_name);
		nlp = &nl->pNext;
	}
	*nlp = NULL;

	/*
	 * Return the result.
	 */
	res.errno = 0;
	closedir(dirp);
	return(&res);
}
