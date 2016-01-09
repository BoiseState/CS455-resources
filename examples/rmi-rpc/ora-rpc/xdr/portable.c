#include <rpc/xdr.h>
#include <stdio.h>

short           sarray[] = {1, 2, 3, 4};

main()
{
  FILE           *fp;
  XDR             xdrs;
  int             i;

  /*
   * Encode the 4 shorts.
   */
  fp = fopen("data", "w");
  xdrstdio_create(&xdrs, fp, XDR_ENCODE);
  for (i = 0; i < 4; i++) if (xdr_short(&xdrs, &(sarray[i])) == FALSE)
    fprintf(stderr, "error writing to stream\n");

  xdr_destroy(&xdrs);
  fclose(fp);

  /*
   * Decode the 4 shorts.
   */
  fp = fopen("data", "r");
  xdrstdio_create(&xdrs, fp, XDR_DECODE);
  for (i = 0; i < 4; i++) if (xdr_short(&xdrs, &(sarray[i])) == FALSE)
    fprintf(stderr, "error reading stream\n"); else printf("%d\n", sarray[i]);

  xdr_destroy(&xdrs);
  fclose(fp);
}
