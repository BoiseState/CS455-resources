/* @(#)remote_sort.c	2.1 88/08/11 4.0 RPCSRC */
/*
 * Modified by Amit Jain
 * remote_sort.c
 * Client side application which sorts argc, argv.
 */
#include <stdio.h>
#include <rpc/rpc.h>
#include "sort.h"

int main (int argc, char **argv)
{
	char *machinename;
	struct sortstrings args, res;
	int i;

	if (argc < 3) {
		fprintf(stderr, "usage: %s machinename [s1 ...]\n", argv[0]);
		exit(1);
	}
	machinename = argv[1];
	args.ss.ss_len = argc - 2;     /* substract off progname, machinename */
	args.ss.ss_val = &argv[2];
	res.ss.ss_val = (char **)NULL;

	if ((i = callrpc(machinename, SORTPROG, SORTVERS, SORT, (xdrproc_t) xdr_sortstrings, 
					(char *)&args, (xdrproc_t) xdr_sortstrings, (char *)&res)))
	{
	    fprintf(stderr, "%s: call to sort service failed. ", argv[0]);
	    clnt_perrno(i);
	    fprintf(stderr, "\n");
	    exit(1);
	}

	for (i = 0; i < res.ss.ss_len; i++) {
		printf("%s\n", res.ss.ss_val[i]);
	}

	/* should free res here */
	exit(0);
}

