#include <signal.h>
#include <stdio.h>
#include <rpc/rpc.h>
#include <string.h>
#include <pwd.h>
#include "dim.h"
#define USAGE() { fprintf(stderr, "Usage: %s ", argv[0]); \
	fprintf(stderr, "\t-t [udp|tcp]\t\toverride transport default\n"); \
	fprintf(stderr, "\t\t-a imageName \"comments\" width height depth compressType"); \
	fprintf(stderr, "\n\t\t\t\t\tadd an image from file 'imageName'\n"); \
	fprintf(stderr, "\t\t-d imageName\t\tdelete an image\n"); \
	fprintf(stderr, "\t\t-x imageName\t\textract an image to file 'imageName'\n"); \
	fprintf(stderr, "\t\t-l\t\t\tlist contents of archive\n"); \
	exit(1); }
#define PRINTHEAD(pI) { \
	printf("name:\t%s\n\towner: %s\n\tcomments: %s\n\tdate: %s\n", \
		pI->sN, pI->sO, pI->sC, pI->sD); \
	printf("\tbytes: %d\twidth: %d\theight: %d\tdepth: %d\tcompress: %d\n", \
		pI->b, pI->x, pI->y, pI->d, pI->c); }

enum clnt_stat cs;
sigpipe()
{
  signal(SIGPIPE, SIG_IGN);
  /* note, clnt_stat is not set to RPC_CANTSEND or _CANTRECV until some time after the signal */
  fprintf(stderr, "%s: %s\n", "SIGPIPE, pipe broken:", clnt_sperrno(cs));
}
image          *readImage();
FILE           *fp;

main(argc, argv)
  int             argc;
  char           *argv[];
{
  CLIENT         *clnt;
  pStr           *expectEmpty;	/* a single NULL if RPC succeeded */
  imageList      *pIL;
  image          *pI;
  imageStat      *pIS;
  pStr            sImageName;
  int             arg;
  char           *transport = DEFAULT_TRANSPORT;

  /*
   * Look thru the command line to see if a transport is specified.
   */
  if (argc < 2)
    USAGE();
  for (arg = 1; arg < argc; arg++) {
    if (!strcmp(argv[arg], "-t")) {
      if ((argc < 4) || (strcmp("udp", argv[++arg]) && strcmp("tcp", argv[arg])))
        USAGE();
      (void) strcpy(transport, argv[arg]);
    }
  }
  /*
   * Attempt to open a client handle to the SERVERHOST.	We could use a
   * broadcast scheme to look for the closest server.
   */
  clnt = clnt_create(SERVERHOST, DIMSERVER, DIMVERS, transport);
  if (clnt == NULL) {
    clnt_pcreateerror(SERVERHOST);
    exit(1);
  }
  /*
	* Be prepared to handle broken pipes.
	*/
  signal(SIGPIPE, sigpipe);
  /*
   * Parse the command line, doing RPCs as requested.  I don't bother
	* xdr_free()'ing anything as it's a one-shot deal.
   */
  for (arg = 1; arg < argc; arg++) {
    if (argv[arg][0] != '-')
		USAGE();
    switch (argv[arg][1]) {
    case 't':
      arg++;
      break;
    case 'a':
      if ((argc - (++arg) < 6) || !(pI = readImage(argv, &arg)))
        USAGE();
      expectEmpty = add_1(pI, clnt);	/* assume RPCSRC4.0, 1 arg */
      if (expectEmpty == NULL) {
  fprintf(stderr, "%s: %s\n", "SIGPIPE, pipe broken:", clnt_sperrno(cs));
		  clnt_perror(clnt, "remote call failed:");
      } else
		  fprintf(stderr, "%s", *expectEmpty);
      break;
    case 'd':
      if (argc - (++arg) < 1)
        USAGE();
      sImageName = (pStr) strdup(argv[arg]);
      expectEmpty = delete_1(&sImageName, clnt);
      if (expectEmpty == NULL)
        clnt_perror(clnt, "remote call failed:");
      else 
        fprintf(stderr, "%s", *expectEmpty);
      break;
    case 'x':
      if (argc - (++arg) < 1)
        USAGE();
      sImageName = (pStr) strdup(argv[arg]);
      pIS = extract_1(&sImageName, clnt);
      if (pIS == NULL) {
        clnt_perror(clnt, "remote call failed:");
        exit(1);
      }
      if (pIS->status[0] != NULL) {
        fprintf(stderr, "%s\n", pIS->status);
        exit(1);
      }
      (void) writeImage(pIS->pImage, sImageName);
      break;
    case 'l':{
        if (!(pIL = list_1((void *) NULL, clnt)))
          clnt_perror(clnt, "remote call failed:");
        else
          for (pI = pIL->pImage; pIL->pNext; pIL = pIL->pNext, pI = pIL->pImage)
            PRINTHEAD(pI);
        break;
      }
    default:
      USAGE();
    }
  }
  clnt_destroy(clnt);
}

/*
 * If I used the rpcgen.new released with ONC's TIRPC, I would not need this
 * packaging function; the call to add_1() could take multiple parameters, by
 * value and/or reference
 */
image          *
readImage(argv, pArg)
  char          **argv;
  int            *pArg;
{
  static image    im;
  char            buffer[MAXBUF];
  char            null = NULL;
  u_int           reallyRead;
  u_int           imageSize = 0;

  /*
   * Build the header information then look at stdin for data.
   */
  im.sN = (pStr) strdup(argv[*pArg]);
  im.sO = UIDTONAME(getuid());
  im.sC = (pStr) strdup(argv[++*pArg]);
  im.x = atoi(argv[++*pArg]);
  im.y = atoi(argv[++*pArg]);
  im.d = atoi(argv[++*pArg]);
  im.c = atoi(argv[++*pArg]);
  im.sD = &null;	/* don't forget to terminate those empty strings! */
  im.data.data_val = (char *)malloc(0);

  if (!(fp = fopen(im.sN, "r"))) {
    fprintf(stderr, "error opening imageName \"%s\" for reading\n", im.sN);
    return (0);
  }
  while (reallyRead = fread(buffer, 1, MAXBUF, fp)) {
    im.data.data_val = (char *)realloc(im.data.data_val,imageSize+reallyRead);
    (void) bcopy(buffer, im.data.data_val + imageSize, reallyRead);
    imageSize += reallyRead;
  }
  im.b = im.data.data_len = imageSize;
  fclose(fp);
  return (&im);
}

writeImage(pImage, sImageName)
  image          *pImage;
  pStr            sImageName;
{
  if (!(fp = fopen(sImageName, "w"))) {
    fprintf(stderr, "error opening imageName \"%s\" for writing\n", sImageName);
    return (1);
  }
  PRINTHEAD(pImage);
  if (fwrite(pImage->data.data_val, 1, pImage->data.data_len, fp)
      != pImage->data.data_len) {
    fprintf(stderr, "error writing imageName \"%s\" data\n", sImageName);
    fclose(fp);
    return (1);
  }
  fclose(fp);
  return (0);
}
