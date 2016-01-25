#include <stdio.h>
#include <rpc/rpc.h>
#include <string.h>
#include <pwd.h>
#include "dim.h"
#define USAGE() { fprintf(stderr, "Usage: %s ", argv[0]); \
	fprintf(stderr, "\t-c [unix|des]\t\toverride AUTH_NONE default authentication\n"); \
	fprintf(stderr, "\t\t-t [udp|tcp]\t\toverride transport default\n"); \
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

char           *my_clnt_sperror();
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
	* Before we place an RPC, we set the authentication choice.
	*/
  for (arg = 1; arg < argc; arg++) {
    if (!strcmp(argv[arg], "-c")) {
      if ((argc < 4) || (strcmp("unix", argv[++arg]) && strcmp("des", argv[arg])))
        USAGE();
		if (!strcmp(argv[arg], "unix"))
        clnt->cl_auth = authunix_create_default();
		else {
        char servername[MAXNETNAMELEN+1];
        getnetname(servername); /* good for root or user */
		  /* authdes_create() obsoleted by AUTHdes_seccreate() */
        clnt->cl_auth = authdes_create(servername, 60, NULL, NULL);
      }
    }
  }
  /*
   * Parse the command line, doing RPCs as requested.  I don't bother
	* xdr_free()'ing anything as it's a one-shot deal.
   */
  for (arg = 1; arg < argc; arg++) {
    if (argv[arg][0] != '-')
		USAGE();
    switch (argv[arg][1]) {
    case 'c':
    case 't':
      arg++;
      break;
    case 'a':
      if ((argc - (++arg) < 6) || !(pI = readImage(argv, &arg)))
        USAGE();
      expectEmpty = add_1(pI, clnt);	/* assume RPCSRC4.0, 1 arg */
      if (expectEmpty == NULL)
		  fputs(my_clnt_sperror(clnt, "remote call failed"), stderr);
      else
		  fprintf(stderr, "%s", *expectEmpty);
      break;
    case 'd':
      if (argc - (++arg) < 1)
        USAGE();
      sImageName = (pStr) strdup(argv[arg]);
      expectEmpty = delete_1(&sImageName, clnt);
      if (expectEmpty == NULL)
		  fputs(my_clnt_sperror(clnt, "remote call failed"), stderr);
      else
        fprintf(stderr, "%s", *expectEmpty);
      break;
    case 'x':
      if (argc - (++arg) < 1)
        USAGE();
      sImageName = (pStr) strdup(argv[arg]);
      pIS = extract_1(&sImageName, clnt);
      if (pIS == NULL) {
		  fputs(my_clnt_sperror(clnt, "remote call failed"), stderr);
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
          fputs(my_clnt_sperror(clnt, "remote call failed"), stderr);
        else
          for (pI = pIL->pImage; pIL->pNext; pIL = pIL->pNext, pI = pIL->pImage)
            PRINTHEAD(pI);
        break;
      }
    default:
      USAGE();
    }
  }
  auth_destroy(clnt->cl_auth);
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


extern char *sys_errlist[]; /* pick-up some error information */
static char *auth_errmsg();

/*
 * Print reply error info
 */
char *
my_clnt_sperror(rpch, s)
CLIENT *rpch;
char *s;
{
	struct rpc_err e;
	void clnt_perrno();
	char *err;
	static char buffer[MAXSTR];
	char *pBuf = buffer;

	CLNT_GETERR(rpch, &e);

	/* 
	 * Build up a return message.
	 */
	(void) sprintf(pBuf, "%s: ", s);  
	pBuf += strlen(pBuf);

	(void) strcpy(pBuf, clnt_sperrno(e.re_status));  
	pBuf += strlen(pBuf);

	switch (e.re_status) {
	case RPC_SUCCESS:				/* we will make better use of these later */
	case RPC_CANTENCODEARGS:
	case RPC_CANTDECODERES:
	case RPC_TIMEDOUT:     
	case RPC_PROGUNAVAIL:
	case RPC_PROCUNAVAIL:
	case RPC_CANTDECODEARGS:
	case RPC_SYSTEMERROR:
	case RPC_UNKNOWNHOST:
	case RPC_UNKNOWNPROTO:
	case RPC_PMAPFAILURE:
	case RPC_PROGNOTREGISTERED:
	case RPC_FAILED:
		break;

	case RPC_CANTSEND:
	case RPC_CANTRECV:
		(void) sprintf(pBuf, "; errno = %s", sys_errlist[e.re_errno]); 
		pBuf += strlen(pBuf);
		break;

	case RPC_VERSMISMATCH:
		(void) sprintf(pBuf,
			"; low version = %lu, high version = %lu", 
			e.re_vers.low, e.re_vers.high);
		pBuf += strlen(pBuf);
		break;

	case RPC_AUTHERROR:
		err = auth_errmsg(e.re_why);
		(void) sprintf(pBuf,"; why = ");
		pBuf += strlen(pBuf);
		if (err != NULL) {
			(void) sprintf(pBuf, "%s",err);
		} else {
			(void) sprintf(pBuf,
				"(unknown authentication error - %d)",
				(int) e.re_why);
		}
		pBuf += strlen(pBuf);
		if  (e.re_why == AUTH_TOOWEAK) {
         /*
	       * Tell user to add authentication info to the handle and try again.
	       */
			switch (rpch->cl_auth->ah_cred.oa_flavor) {
			case NULL: /* was AUTH_NON, try AUTH_UNIX */
				(void) sprintf(pBuf, ", try -c unix");
				break;
			case AUTH_UNIX:
				(void) sprintf(pBuf, ", try -c des");
				break;
			case AUTH_DES: /* already tried DES... */
				(void) sprintf(pBuf, ", try -c unix");
				break;
			default: /* must have some foreign credential type... */
				(void) sprintf(pBuf, "unknown, unsuccessful credentials");
				break;
			}
			pBuf += strlen(pBuf);
		}
		break;

	case RPC_PROGVERSMISMATCH:
		(void) sprintf(pBuf, 
			"; low version = %lu, high version = %lu", 
			e.re_vers.low, e.re_vers.high);
		pBuf += strlen(pBuf);
		break;

	default:	/* unknown */
		(void) sprintf(pBuf, 
			"; s1 = %lu, s2 = %lu", 
			e.re_lb.s1, e.re_lb.s2);
		pBuf += strlen(pBuf);
		break;
	}
	(void) sprintf(pBuf, "\n");
	return(buffer) ;
}

struct auth_errtab {
   enum auth_stat status;
   char *message;
};

static struct auth_errtab auth_errlist[] = {
   { AUTH_OK,
      "Authentication OK" },
   { AUTH_BADCRED,
      "Invalid client credential (security seal broken)" },
   { AUTH_REJECTEDCRED,
      "Server rejected credential, client must begin new session" },
   { AUTH_BADVERF,
      "Invalid client verifier (security seal broken)" },
   { AUTH_REJECTEDVERF,
      "Server rejected verifier (verifier expired or was replayed)" },
   { AUTH_TOOWEAK,
      "Server denies access, client credential too weak" },
   { AUTH_INVALIDRESP,
      "Invalid server verifier (server response rejected by this client)" },
   { AUTH_FAILED,
      "Failed (unspecified error)" },
};


static char *
auth_errmsg(stat)
   enum auth_stat stat;
{
   int i;

   for (i = 0; i < sizeof(auth_errlist)/sizeof(struct auth_errtab);
i++) {
      if (auth_errlist[i].status == stat) {
         return(auth_errlist[i].message);
      }
   }
   return(NULL);
}

