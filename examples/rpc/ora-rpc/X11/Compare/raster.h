/* define some image file format stuff (vestigal Sunview stuff) */
typedef struct colormap_t {
        int              type;
        int              length;
        unsigned char   *map[3];
} colormap_t;

typedef struct rasterfile {
        int     ras_magic;              /* magic number */
        int     ras_width;              /* width (pixels) of image */
        int     ras_height;             /* height (pixels) of image */
        int     ras_depth;              /* depth (1, 8, or 24 bits) of pixel */
        int     ras_length;             /* length (bytes) of image */
        int     ras_type;               /* type of file; see RT_* below */
        int     ras_maptype;            /* type of colormap; see RMT_* below */
        int     ras_maplength;          /* length (bytes) of following map */
        /* color map follows for ras_maplength bytes, followed by image */
} rasterfile;
#define RAS_MAGIC       0x59a66a95

        /* Sun supported ras_type's */
#define RT_OLD          0       /* Raw pixrect image in 68000 byte order */
#define RT_STANDARD     1       /* Raw pixrect image in 68000 byte order */
#define RT_BYTE_ENCODED 2       /* Run-length compression of bytes */
#define RT_FORMAT_RGB   3       /* XRGB or RGB instead of XBGR or BGR */
#define RT_FORMAT_TIFF  4       /* tiff <-> standard rasterfile */
#define RT_FORMAT_IFF   5       /* iff (TAAC format) <-> standard rasterfile */
#define RT_EXPERIMENTAL 0xffff  /* Reserved for testing */

        /* Sun registered ras_maptype's */
#define RMT_RAW         2       /* Sun supported ras_maptype's */
#define RMT_NONE        0       /* ras_maplength is expected to be 0 */
#define RMT_EQUAL_RGB   1        /* red[ras_maplength/3],green[],blue[] */

#define INITRAST(r, w, h) { \
  r.ras_magic = RAS_MAGIC; \
  r.ras_width = w; \
  r.ras_height = h; \
  r.ras_depth = 8; \
  r.ras_length = w*h; \
  r.ras_type = RT_STANDARD; \
  r.ras_maptype = RMT_NONE; \
  r.ras_maplength = 0; }
