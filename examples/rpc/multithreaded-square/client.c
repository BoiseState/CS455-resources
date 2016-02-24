
/*
client
*/

#include "memory.h" /* for memset */
#include "square.h"
#include "stdio.h"
#include "stdlib.h"
#include "rpc/pmap_clnt.h"
#include "string.h"
#include "memory.h"
#include "sys/socket.h"
#include "netinet/in.h"

int main (int argc,char **argv)
{
	CLIENT *cl;
	square_in in;
	square_out out;
	if (argc != 3 ) { 
		printf ("Usage : client \"hostname\" \"integer_values\"\n");
		exit(1);
	}
	cl = clnt_create(argv[1],SQUARE_PROG,SQUARE_VERS,"tcp");
	if (cl == NULL) {
		clnt_perror (cl, "call failed");
		exit (1);
	}
	in.arg1 = atol(argv[2]);
	if (squareproc_2(&in,&out,cl) != RPC_SUCCESS)
	{
		printf("%s\n", clnt_sperror(cl,argv[1]));
		exit(1);
	}
		printf("result: %ld\n",out.res1);
		exit(0);
}


/* vim: set ts=4 sw=4 */
