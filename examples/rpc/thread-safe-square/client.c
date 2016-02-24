#include	"square.h"
#include	"ourhdr.h"

int
main(int argc, char **argv)
{
	CLIENT		*cl;
	square_in	in;
	square_out	out;

	if (argc != 3)
		err_quit("usage: client <hostname> <integer-value>");

	cl = clnt_create(argv[1], SQUARE_PROG, SQUARE_VERS, "tcp");
	if (cl == NULL) {
		/* Couldn't establish connection with server.
		 * Print error message and die.
		 */
		clnt_pcreateerror(argv[1]);
		exit(1);
	}


	in.arg1 = atol(argv[2]);
	if (squareproc_2(&in, &out, cl) != RPC_SUCCESS)
		err_quit("%s", clnt_sperror(cl, argv[1]));

	printf("result: %ld\n", out.res1);
	exit(0);
}
