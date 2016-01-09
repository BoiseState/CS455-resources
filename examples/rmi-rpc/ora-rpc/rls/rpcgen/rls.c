/*
 * rls.c: remote directory listing clien
*/
#include <stdio.h>
#include <rpc/rpc.h>
#include "rls.h"

extern int errno;

main (argc, argv)
	int argc;
	char *argv[];
{
	CLIENT *cl;
	char *server;
	char *dir;
	readdir_res *result;
	namelist nl;

	if (argc != 3) {
		fprintf(stderr, "usage: %s host directory\n", argv[0]);
		exit(1);
	}

	/*
	 * remember what our command line arguments refers to
	 */
	server = argv[1];
	dir = argv[2];

	/*
	 * create client "handle" used for calling MESSAGEPROG on the
	 * server designated on command line.  we tell RPC package
	 * to use the "tcp" prtocol when contacting the server
	 */
#ifdef DEBUG
	cl = (CLIENT *)1;
#else
	 cl = clnt_create(server, DIRPROG, DIRVERS, "tcp");
#endif DEBUG
	 if (cl == NULL) {
		/*
		 * couldn't establish connection with server
		 * print error message and die.
		 */
		clnt_pcreateerror(server);
		exit(1);
	}

	/*
	 * call the remote procedure readdir on the server
	 */
	result = readdir_1(&dir, cl);
	if (result == NULL) {
		/*
		 * an error occurred while calliing the server,
		 * print error message and die...
		 */
		clnt_perror(cl, server);
		exit(1);
	}

	/*
	 * okay, we successfully called the reote procedure
	 */
	if (result->errno != 0) {
		/*
		 * a remote system error occurred,
		 * print error message and die
		 */
		errno = result->errno;
		perror(dir);
		exit(1);
	}

	/* 
	 * successfully got a directory listing, print it out
	 */
	for (nl = result->readdir_res_u.list; nl != NULL; nl = nl->pNext) {
		printf("%s\n", nl->name);
	}
	exit(0);
}

