#include <stdio.h>
#include <rpc/rpc.h>
#include "rls.h"

static void     dispatch();	/* the server program - does dispatching */
extern bool_t   xdr_dir();
struct timeval  tval;
static int      count = 0;

/*
 * Register the service, then wait for requests without consuming resources.
 */
main()
{
	SVCXPRT        *transp;

	(void) pmap_unset(DIRPROG, DIRVERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		(void) fprintf(stderr, "cannot create udp service.\n");
		exit(1);
	}
	if (!svc_register(transp, DIRPROG, DIRVERS, dispatch, IPPROTO_UDP)) {
		(void) fprintf(stderr,
			   "unable to register (DIRPROG, DIRVERS, udp).\n");
		exit(1);
	}
	tval.tv_sec = tval.tv_usec = 0;

	svc_run();
	(void) fprintf(stderr, "svc_run returned\n");
	exit(1);
}

/*
 * Decode the requested service and provide it.
 */
static void
dispatch(rqstp, transp)
	struct svc_req *rqstp;
	SVCXPRT        *transp;
{
	char            dir[DIR_SIZE];
	enum clnt_stat  clnt_stat;
	CLIENT         *client;

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply(transp, xdr_void, 0);
		return;

	case READDIR:
		if (!svc_getargs(transp, xdr_dir, dir)) {
			svcerr_decode(transp);
			return;
		}
		count++;

		/*
		 * Parent goes back to listening for requests, child does the
		 * work (doesn't that sound familiar!
		 */
		if (fork())
			return;
		read_dir(dir);
		printf("%d\n", count);

		/*
		 * Send the reply back. Again, no need for clean-up - just
		 * die gracefully
		 */
		if (!svc_sendreply(transp, xdr_dir, dir)) {
			svcerr_systemerr(transp);
			_exit(-1);
		}
		_exit(0);

		/*
		 * put more procedures here...  case A: case Z:
		 */


	default:
		svcerr_noproc(transp);
		return;
	}
}
