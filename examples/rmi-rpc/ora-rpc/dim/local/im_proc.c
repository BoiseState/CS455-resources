#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <pwd.h>
#include "im.h"

#define FGETS(ptr, max, fp) { fgets(ptr, max, fp); ptr[strlen(ptr)-1] = NULL; }
#define READHEADER(n, o, c, d) \
	{ FGETS(n,MAXSTR,fp); FGETS(o,MAXSTR,fp); \
	FGETS(c,MAXSTR,fp); FGETS(d,MAXSTR,fp); }

FILE           *fp;
imageList      *iLAllocOne();
image          *iAllocOne();

pStr
add(argp)
  image          *argp;
{
  static pStr     result;
  static char     msg[MAXSTR];
  static char     N[MAXSTR], O[MAXSTR], C[MAXSTR], D[MAXSTR];
  char            head[MAXSTR];
  int             fstat, b, x, y, d, c;
  time_t          tloc;
  result = msg;
  msg[0] = NULL;

  if (!(fp = fopen(SERVERDB, "r"))) {
    sprintf(msg, "cannot open SERVERDB %s for reading\n", SERVERDB);
    return ((pStr) result);
  }
  /*
   * First make sure such an image isn't already archived.
   */
  while ((fstat = fscanf(fp, "%d%d%d%d%d\n", &b, &x, &y, &d, &c)) == 5) {
    READHEADER(N, O, C, D);
    if (!strcmp(N, argp->sN))
      break;
    fseek(fp, (long) b, 1);
  }
  switch (fstat) {
  case EOF:			/* not found - that's good */
    fclose(fp);
    if (!(fp = fopen(SERVERDB, "a"))) {
      sprintf(msg, "cannot open SERVERDB %s to append\n", SERVERDB);
      fclose(fp);
      return ((pStr) result);
    }
    break;
  case 5:			/* there already is one! */
    sprintf(msg, "%s archive already has a \"%s\"\n", SERVERDB, argp->sN);
    fclose(fp);
    return ((pStr) result);
  default:			/* not a clean tail... tell user and try to
				 * recover */
    repairDB(msg);
    fclose(fp);
    return ((pStr) result);
  }
  /*
   * Get the date, add the image header and data, then return.
   */
  time(&tloc);
  sprintf(head, "%d %d %d %d %d\n%s\n%s\n%s\n%s",
	  argp->data.data_len, argp->x, argp->y, argp->d, argp->c,
	  argp->sN, argp->sO, argp->sC, (char *) ctime(&tloc));
  if ((fwrite(head, 1, strlen(head), fp) != strlen(head))
      || (fwrite(argp->data.data_val, 1, argp->data.data_len, fp)
	  != argp->data.data_len)) {
    sprintf(msg, "failed write to SERVERDB %s\n", SERVERDB);
  }
  fclose(fp);
  return ((pStr) result);
}

/*
 * This is included for the sake of completeness but is brute-force.
 */
pStr
delete(argp)
  pStr            argp;
{
  FILE           *fpp;
  int             fstat;
  static pStr     result;
  static char     msg[MAXSTR];
  char            N[MAXSTR], O[MAXSTR], C[MAXSTR], D[MAXSTR];
  char           *buffer;
  int             bufSize, bytesRead, b, x, y, d, c;
  int             seekPt = 0;

  msg[0] = NULL;
  result = msg;

  if (!(fp = fopen(SERVERDB, "r"))) {
    sprintf(msg, "cannot open SERVERDB %s for reading\n", SERVERDB);
    return ((pStr) result);
  }
  /*
   * Look thru the DB for the named image.
   */
  while ((fstat = fscanf(fp, "%d%d%d%d%d\n", &b, &x, &y, &d, &c)) == 5) {
    READHEADER(N, O, C, D);
    fseek(fp, (long) b, 1);	/* fp stops at next entry */
    if (!strcmp(N, argp))
      break;
    seekPt = ftell(fp);
  }
  switch (fstat) {
  case EOF:			/* not found */
    sprintf(msg, "%s not found in archive\n", argp);
    break;
  case 5:	/* This is the one! Remove it by copying the bottom up. */
    bufSize = MIN(MAX(1, b), MAXBUF);
    buffer = (char *) malloc(bufSize);
    fpp = fopen(SERVERDB, "r+");
    fseek(fpp, seekPt, 0);	/* fpp is at selected image */
    while (!feof(fp)) {
      bytesRead = fread(buffer, 1, bufSize, fp);
      fwrite(buffer, 1, bytesRead, fpp);
    }
    seekPt = ftell(fpp);
    fclose(fpp);
    truncate(SERVERDB, (off_t) seekPt);
    break;
  default:			/* not a clean tail... */
    repairDB(msg);
  }
  fclose(fp);
  return ((pStr) result);
}


pStr
extract(argp, ppIm)
  pStr            argp;
  image         **ppIm;
{
  int             fstat;
  static pStr     result;
  static char     msg[MAXSTR];
  image          *pIm;

  result = msg;
  msg[0] = NULL;

  if (!(fp = fopen(SERVERDB, "r"))) {
    sprintf(msg, "cannot open SERVERDB %s for reading\n", SERVERDB);
    return ((pStr) result);
  }
  /*
   * Look thru the DB for the named image.
   */
  pIm = *ppIm = iAllocOne();

  while ((fstat = fscanf(fp, "%d%d%d%d%d\n", &(pIm->b), &(pIm->x), &(pIm->y),
			 &(pIm->d), &(pIm->c))) == 5) {
    READHEADER(pIm->sN, pIm->sO, pIm->sC, pIm->sD);

    if (!strcmp(pIm->sN, argp))
      break;
    fseek(fp, (long) pIm->b, 1);
  }
  switch (fstat) {
  case EOF:			/* not found */
    sprintf(msg, "%s not found in archive\n", argp);
    break;
  case 5:			/* this is the one! */
    pIm->data.data_len = pIm->b;
    pIm->data.data_val = (char *) malloc(pIm->b);
    if (fread(pIm->data.data_val, 1, pIm->data.data_len, fp)
	!= pIm->data.data_len) {
      sprintf(msg, "couldn't read all of %s\n", argp);
		repairDB(msg);
    }
    break;
  default:			/* not a clean tail... */
    repairDB(msg);
  }
  fclose(fp);
  return ((pStr) result);
}

static imageList *pIList = NULL;

imageList      *
list()
{
  imageList      *pIL;
  int             fstat;

  /*
   * Build a list.
   */
  pIL = pIList = iLAllocOne();

  if (!(fp = fopen(SERVERDB, "r"))) {
    sprintf(pIL->pImage->sN, "cannot open SERVERDB %s for reading\n", SERVERDB);
    return (pIList);
  }
  while ((fstat = fscanf(fp, "%d%d%d%d%d\n", &(pIL->pImage->b),
			 &(pIL->pImage->x), &(pIL->pImage->y),
			 &(pIL->pImage->d), &(pIL->pImage->c))) == 5) {
    READHEADER(pIL->pImage->sN, pIL->pImage->sO,
	       pIL->pImage->sC, pIL->pImage->sD);

    fseek(fp, (long) pIL->pImage->b, 1);

    pIL->pNext = iLAllocOne();	/* hang an empty one on the end */
    pIL = pIL->pNext;
  }
  if (fstat != EOF) {		/* not a clean tail... */
    repairDB(pIL->pImage->sN);
  }
  fclose(fp);
  return (pIList);
}

/*
 * The next two routines are just image linked-list maint. stuff.
 */
imageList      *
iLAllocOne()
{				/* allocate one imageList structure */
  imageList      *pIL = (imageList *) malloc(sizeof(imageList));
  pIL->pImage = iAllocOne();
  pIL->pNext = NULL;
  return (pIL);
}
image          *
iAllocOne()
{				/* allocate one image structure */
  image          *pI = (image *) malloc(sizeof(image));
  pI->sN = (pStr) malloc(MAXSTR);
  pI->sN[0] = NULL;
  pI->sO = (pStr) malloc(MAXSTR);
  pI->sO[0] = NULL;
  pI->sC = (pStr) malloc(MAXSTR);
  pI->sC[0] = NULL;
  pI->sD = (pStr) malloc(MAXSTR);
  pI->sD[0] = NULL;
  pI->data.data_len = 0;
  pI->data.data_val = NULL;
  return (pI);
}

repairDB(s)			/* doesn't do much, yet... */
  pStr            s;
{
  sprintf(s, "SERVERDB %s data hosed, repaired\n", SERVERDB);
}
