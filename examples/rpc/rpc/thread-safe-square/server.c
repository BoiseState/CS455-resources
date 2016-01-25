#include	"square.h"
#include	"ourhdr.h"
#include	<pthread.h>

bool_t
squareproc_2_svc(square_in *inp, square_out *outp, struct svc_req *rqstp)
{
	printf("thread %ld started, arg = %ld\n",
		   pthread_self(), inp->arg1);
	sleep(4);
	outp->res1 = inp->arg1 * inp->arg1;
	printf("thread %ld done\n", pthread_self());
	return(TRUE);
}

int
square_prog_2_freeresult(SVCXPRT *transp, xdrproc_t xdr_result,
						 caddr_t result)
{
	xdr_free(xdr_result, result);
	return(1);
}
