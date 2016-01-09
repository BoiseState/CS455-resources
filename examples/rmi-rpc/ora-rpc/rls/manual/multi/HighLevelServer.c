#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include "rls.h"

static int count = 0;

f_read_dir(dir)
char *dir;
{
	char ccount[32];

	/*
	 * If you are the parent servber process, return to the
	 * svc_run() loop directly, otherwise, read the directory
	 * and pass-back results - as the contents of dir.
	 */
	if (fork()) _exit(0);
	read_dir(dir);

	/*
	 * Stuff the count on the end so you know what order
	 * processing was performed in.
	 */
	printf("%d\n", count++); fflush(stdout);
	sprintf(ccount, "%d", count);
	strcat(dir, ccount);
	return((int)dir);
}

main()
{
	extern bool_t xdr_dir();

	registerrpc(DIRPROG, DIRVERS, READDIR,
			f_read_dir, xdr_dir, xdr_dir);

	svc_run();
	(void) fputs("svc_run died...\n");exit(1);
}
