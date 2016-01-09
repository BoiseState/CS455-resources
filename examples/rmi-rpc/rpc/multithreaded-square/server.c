
/*
server
*/

#include "square.h"
#include "stdio.h"
#include "stdlib.h"
#include "rpc/pmap_clnt.h"
#include "string.h"
#include "memory.h"
#include "sys/socket.h"
#include "netinet/in.h"

int request=0;

bool_t squareproc_2_svc(square_in *inp, square_out *outp, struct svc_req *rqstp)
{
	printf("Thread id = '%X' started, arg = %ld\n",pthread_self(),inp->arg1);
	sleep(2);
	outp->res1=inp->arg1*inp->arg1;
	printf("Thread id = '%X' is done %ld \n",pthread_self(),outp->res1);
	return(TRUE);
}

int square_prog_2_freeresult(SVCXPRT *transp,xdrproc_t xdr_result, caddr_t result)
{
	xdr_free(xdr_result,result);
	return(1);
}


/* vim: set ts=4 sw=4 */
