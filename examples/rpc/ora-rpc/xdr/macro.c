/* Note - I am attempting to resolve the problems allocating streams
   under SunOS4.1.X. I shall post a corrected form of this code if
   necessary. - John Bloomer, jbloomer@crd.ge.com 1-Feb-92 */

#include <rpc/rpc.h>
#include <stdio.h>

short           sarray[] = {1, 2, 3, 4};

main()
{
  FILE           *fp;
  register XDR    xdrs;
  register long  *buf;
  int             i;

  /*
   * Encode the 4 shorts.
   */
  fp = fopen("data", "w");
  xdrstdio_create(&xdrs, fp, XDR_ENCODE);
  buf = XDR_INLINE(&xdrs, BYTES_PER_XDR_UNIT * 4);
  if (buf == NULL) {
    fprintf(stderr, "error allocating stream space\n");
    exit(1);
  } else {
    for (i = 0; i < 4; i++)
      IXDR_PUT_SHORT(buf, sarray[i]);
  }

  xdr_destroy(&xdrs);
  fclose(fp);

  /*
   * Decode the 4 shorts.
   */
  fp = fopen("data", "r");
  xdrstdio_create(&xdrs, fp, XDR_DECODE);
  buf = XDR_INLINE(&xdrs, BYTES_PER_XDR_UNIT * 4);
  if (buf == NULL) {
    fprintf(stderr, "error reading stream\n");
    exit(1);
  } else {
    for (i = 0; i < 4; i++)
      printf("%d\n", IXDR_GET_SHORT(buf));
  }

  xdr_destroy(&xdrs);
  fclose(fp);
}
