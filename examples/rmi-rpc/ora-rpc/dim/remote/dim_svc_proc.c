#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <pwd.h>
#include <rpc/rpc.h>
#include "dim.h"

#define FGETS(ptr, max, fp) { fgets(ptr, max, fp); ptr[strlen(ptr)-1] = NULL; }
#define READHEADER(n, o, c, d) \
	{ FGETS(n,MAXSTR,fp); FGETS(o,MAXSTR,fp); \
	FGETS(c,MAXSTR,fp); FGETS(d,MAXSTR,fp); }

FILE           *fp;
imageList      *iLAllocOne();
image          *iAllocOne();

void           *
die_1()
{
  svc_unregister(DIMSERVER, DIMVERS);
  exit(0);
}

void           *
restart_1()
{
  svc_unregister(DIMSERVER, DIMVERS);
  (void) execl(SERVERPATH, (char *) 0);
}

pStr           *
add_1(argp, rqstp)
  image          *argp;
  struct svc_req *rqstp;
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
    fprintf(stderr, msg);
    return ((pStr *) & result);
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
      fprintf(stderr, msg);
      fclose(fp);
      return ((pStr *) & result);
    }
    break;
  case 5:			/* there already is one! */
    sprintf(msg, "%s archive already has a \"%s\"\n", SERVERDB, argp->sN);
    fprintf(stderr, msg);
    fclose(fp);
    return ((pStr *) & result);
  default:			/* not a clean tail... tell user and try to recover */
    repairDB(msg);
    fclose(fp);
    return ((pStr *) & result);
  }

  CompressImage(1, argp);  /* compress as specified */
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
    fprintf(stderr, msg);
  }
  fclose(fp);
  return ((pStr *) & result);
}

/*
 * This is included for the sake of completeness but is brute-force.
 */
pStr           *
delete_1(argp, rqstp)
  pStr           *argp;
  struct svc_req *rqstp;
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
    fprintf(stderr, msg);
    return ((pStr *) & result);
  }
  /*
   * Look thru the DB for the named image.
   */
  while ((fstat = fscanf(fp, "%d%d%d%d%d\n", &b, &x, &y, &d, &c)) == 5) {
	 READHEADER(N, O, C, D);
    fseek(fp, (long) b, 1);	/* fp stops at next entry */
    if (!strcmp(N, *argp))
      break;
    seekPt = ftell(fp);
  }
  switch (fstat) {
  case EOF:			/* not found */
    sprintf(msg, "%s not found in archive\n", *argp);
    fprintf(stderr, msg);
    break;
  case 5:			/* This is the one! Remove it by copying the bottom up. */
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
  return ((pStr *) & result);
}

static image   *pIm = NULL;

imageStat      *
extract_1(argp, rqstp)
  pStr           *argp;
  struct svc_req *rqstp;
{
  int             fstat;
  static imageStat iS;
  static char     msg[MAXSTR];

  iS.status = msg;
  iS.pImage = NULL;

  if (!(fp = fopen(SERVERDB, "r"))) {
    sprintf(msg, "cannot open SERVERDB %s for reading\n", SERVERDB);
    fprintf(stderr, msg);
    return (&iS);
  }
  /*
   * Reuse any previously allocated memory, look thru the DB for the named
   * image.
   */
  if (pIm!=NULL) xdr_free(xdr_image, pIm); pIm = iAllocOne();
  iS.pImage = pIm;

  while ((fstat = fscanf(fp, "%d%d%d%d%d\n", &(pIm->b), &(pIm->x), &(pIm->y),
			 &(pIm->d), &(pIm->c))) == 5) {
	 READHEADER(pIm->sN, pIm->sO, pIm->sC, pIm->sD);

    if (!strcmp(pIm->sN, *argp)) break;
    fseek(fp, (long) pIm->b, 1);
  }
  switch (fstat) {
  case EOF:			/* not found */
    sprintf(msg, "%s not found in archive\n", *argp);
    fprintf(stderr, msg);
    break;
  case 5:			/* this is the one! */
    pIm->data.data_len = pIm->b;
    pIm->data.data_val = (char *) malloc(pIm->b);
    if (fread(pIm->data.data_val, 1, pIm->data.data_len, fp)
        != pIm->data.data_len) {
      sprintf(msg, "couldn't read all of %s\n", *argp);
      fprintf(stderr, msg);
		repairDB(msg);
    }
	 CompressImage(-1, pIm);   /* decompress */
    break;
  default:			/* not a clean tail... */
    repairDB(msg);
  }
  fclose(fp);
  return (&iS);
}

static imageList *pIList = NULL;

imageList      *
list_1(argp, rqstp)
  void           *argp;
  struct svc_req *rqstp;
{
  imageList      *pIL;
  int             fstat;

  /*
   * Free any memory previously allocated memory then build a list.
   */
  if (pIList) xdr_free(xdr_imageList, pIList);

  pIL = pIList = iLAllocOne();

  if (!(fp = fopen(SERVERDB, "r"))) {
    sprintf(pIL->pImage->sN, "cannot open SERVERDB %s for reading\n", SERVERDB);
    fprintf(stderr, pIL->pImage->sN);
    pIL->pNext = iLAllocOne();	/* hang an empty one on the end */
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
 * The next four routines are just image linked-list maint. stuff.
 */
imageList      *
iLAllocOne()			/* allocate one imageList structure */
{
  imageList      *pIL = (imageList *) malloc(sizeof(imageList));
  pIL->pImage = iAllocOne();
  pIL->pNext = NULL;
  return (pIL);
}
image          *
iAllocOne()				/* allocate one image structure */
{
  /* Don't abuse the fact that in XDR, xdr_string(XDR_FREE) really
   * deallocates using "#define mem_free(ptr, bsize) free(ptr)", not
   * really looking for NULLs with strlen().  You should bzero() or
	* calloc() for MAXSTR length strings.
   */
  image          *pI = (image *) malloc(sizeof(image));
  pI->sN = (pStr) calloc(MAXSTR, 1);
  pI->sO = (pStr) calloc(MAXSTR, 1);
  pI->sC = (pStr) calloc(MAXSTR, 1);
  pI->sD = (pStr) calloc(MAXSTR, 1);
  pI->data.data_len = 0;
  pI->data.data_val = NULL;
  return (pI);
}

repairDB(s)				/* doesn't do much, yet... */
  pStr            s;
{
  sprintf(s, "SERVERDB %s data hosed, repaired\n", SERVERDB);
  fprintf(stderr, s);
}

CompressImage(d, pIm)   /* compression and decompression */
int	      d;
image      *pIm;
{
  /* omitted */
}
