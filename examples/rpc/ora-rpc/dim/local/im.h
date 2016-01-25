#define MAX(a,b)		(a>b)?a:b
#define MIN(a,b)		(a<b)?a:b
#define UIDTONAME(uid) (char *)strdup((char *)((struct passwd *)getpwuid((int)uid))->pw_name)
/* this must be a writeable NFS volume */
#define SERVERDB    "/home/cortex/projects/vision/archive.dim"
#define LZW_COMP		1	/* Lempel-Ziv-Welch coded data */
#define HUFF_COMP		2	/* Huffman-coded data */
#define AHUFF_COMP	3	/* Adaptive Huffman-coded data */
#define ARITH_COMP	4	/* Arithmetic-coded data, 3-rd order model */
#define DPCM_COMP		5	/* Differential PCM-coded data, h1=1
					 * 1st order */
#define VQ_COMP		6	/* Vector quantized data */
#define DCT_COMP		7	/* Discrete Cosine Transformed data,
					 * JPEG */
#define MAXBUF 8192
#define MAXSTR 256
#define MAXPIX 0x100000
#define MAXIMG 256

typedef char   *pStr;

struct image {
  int             b;
  int             x;
  int             y;
  int             d;
  int             c;
  pStr            sN;
  pStr            sO;
  pStr            sC;
  pStr            sD;
  struct {
    u_int           data_len;
    char           *data_val;
  }               data;
};
typedef struct image image;

typedef struct imageStat imageStat;

struct imageList {
  image          *pImage;
  struct imageList *pNext;
};
typedef struct imageList imageList;

extern pStr     add();
extern pStr     delete();
extern pStr     extract();
extern imageList *list();
