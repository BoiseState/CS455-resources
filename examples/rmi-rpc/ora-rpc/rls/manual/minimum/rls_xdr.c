#include <rpc/rpc.h>
#include "rls.h"
bool_t xdr_dir(xdrs, objp)
XDR *xdrs;
char *objp;
{ return ( xdr_string(xdrs, &objp, DIR_SIZE) ); }
