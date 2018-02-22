#include	"square.h"

square_out *squareproc_1_svc(square_in *inp, struct svc_req *rqstp)
{
	/* out cannot be automatic because we are returning its address */
	static square_out out; 

	out.res1 = inp->arg1 * inp->arg1;
	return(&out);
}
