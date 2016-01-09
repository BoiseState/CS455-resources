/* @(#)sort_server.c	2.1 88/08/11 4.0 RPCSRC */
/* modified by amit jain */

#include <rpc/rpc.h>
#include "sort.h"

static int comparestrings(const void *sp1, const void *sp2)
{
    return (strcmp(*(char **)sp1, *(char **)sp2));
}

struct sortstrings * sort_1_svc(struct sortstrings *ssp, struct svc_req *req)
{
    static struct sortstrings ss_res;

    if (ss_res.ss.ss_val != (str *)NULL)
        free(ss_res.ss.ss_val);

    qsort(ssp->ss.ss_val, ssp->ss.ss_len, sizeof (char *), comparestrings);
    ss_res.ss.ss_len = ssp->ss.ss_len;
    ss_res.ss.ss_val = (str *)malloc(ssp->ss.ss_len * sizeof(str *));
    bcopy(ssp->ss.ss_val, ss_res.ss.ss_val,
        ssp->ss.ss_len * sizeof(str *));
    return(&ss_res);
}
