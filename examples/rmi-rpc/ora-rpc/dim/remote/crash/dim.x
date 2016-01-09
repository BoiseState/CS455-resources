/*
 * Some glop we need in all the other C code, not here.
 */
#if RPC_HDR
%#define MAX(a,b)		(a>b)?a:b
%#define MIN(a,b)		(a<b)?a:b
%#define UIDTONAME(uid) (char *)strdup((char *)((struct passwd *)getpwuid((int)uid))->pw_name)
%#define SERVERHOST  "cortex"
%#define SERVERPATH  "/home/cortex/u0/bloomer/src/rpc/advanced/crash/dim_svc"
%#define SERVERDB    "/home/cortex/projects/vision/archive.dim"
%#define DEFAULT_TRANSPORT "tcp"
%#define LZW_COMP		1	/* Lempel-Ziv-Welch coded data */
%#define HUFF_COMP	2	/* Huffman-coded data */
%#define AHUFF_COMP	3	/* Adaptive Huffman-coded data */
%#define ARITH_COMP	4	/* Arithmetic-coded data, 3-rd order model */
%#define DPCM_COMP	5	/* Differential PCM-coded data, h1=1 1st order */
%#define VQ_COMP		6	/* Vector quantized data */
%#define DCT_COMP		7	/* Discrete Cosine Transformed data, JPEG */
#endif

/*
 * These integer constants we need now.
 */
const	MAXBUF = 8192;		/* temporary buffer size maximum */
const	MAXSTR = 256;		/* maximum string length */
const	MAXPIX = 0x100000;/* up to 1M pixels */
const	MAXIMG = 256;		/* maximum Images in the archive */

/*
 * Define stuctures used by client and server.
 */
typedef string  pStr < MAXSTR >;
struct image {
  int             b;		/* total # of bytes, augments data.data_len */
  int             x;		/* width, height in pixels, depth in bytes */
  int             y;
  int             d;
  int             c;		/* 0 if no compression, *_COMP type otherwise */
  pStr            sN;	/* name of the image file */
  pStr            sO;	/* owner, derived from uid */
  pStr            sC;	/* comments, imaging conditions, etc. */
  pStr            sD;	/* date, set at server */
  opaque          data < MAXPIX >;
};
struct imageStat {		/* status string and an image */
	pStr  status;
	image *pImage;
};
struct imageList {		/* a linked-list of images */
	image *pImage;
	imageList *pNext;
};

/*
 * The dim server procedure definition.
 */
program         DIMSERVER {
  version         DIMVERS {
    void          DIE(void) = 1;		/* for maint. */
    void          RESTART(void) = 2;/* for maint. */
    pStr          ADD(image) = 3;	/* -a */
    pStr          DELETE(pStr) = 4;	/* -d */
    imageStat     EXTRACT(pStr) = 5;/* -x */
    imageList     LIST(void) = 6;	/* -t */
  } = 1;
} = 0x20000009;
