#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include "im.h"
#define USAGE() { fprintf(stderr, "Usage: %s ", argv[0]); \
	fprintf(stderr, "\t-a imageName \"comments\" width height depth compressType"); \
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

image          *readImage();
FILE           *fp;

main(argc, argv)
  int             argc;
  char           *argv[];
{
  pStr            expectEmpty;	/* a single NULL if success, else an error string*/
  imageList      *pIL;
  image          *pI;
  pStr            sImageName;
  int             arg;

  /*
   * Parse the command line, doing local procedure calls as requested.
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
      expectEmpty = add(pI);
      if (expectEmpty[0] != NULL)
		  fprintf(stderr, "local call failed: %s", expectEmpty);
      break;
    case 'd':
      if (argc - (++arg) < 1)
        USAGE();
      sImageName = (pStr) strdup(argv[arg]);
      expectEmpty = delete(sImageName);
      if (expectEmpty[0] != NULL)
        fprintf(stderr, "local call failed: %s", expectEmpty);
      break;
    case 'x':
      if (argc - (++arg) < 1)
        USAGE();
      sImageName = (pStr) strdup(argv[arg]);
      expectEmpty = extract(sImageName, &pI);
      if (expectEmpty[0] != NULL)
        fprintf(stderr, "local call failed: %s", expectEmpty);
      else
      (void) writeImage(pI, sImageName);
      break;
    case 'l':{
        if (!(pIL = list((void *) NULL)))
          fprintf(stderr, "local call failed:");
        else
          for (pI = pIL->pImage; pIL->pNext; pIL = pIL->pNext, pI = pIL->pImage)
            PRINTHEAD(pI);
        break;
      }
    default:
      USAGE();
    }
  }
}

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
