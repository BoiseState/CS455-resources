/*
 * rip.x: remote image processing protocol between servers and client.
 * First, Define the tokens referencing image processing functions, used in
 * the op fields.
 */
const           READIMAGE 	= 0;
const           READKERNEL 	= 1;
const           CONVOLVE 	= 2;
const           OPENWINDOW 	= 3;
const           ADDSERVER 	= 4;
const           HELP 		= 5;
const           SUCCESS 	= 0xfd;
const           ERROR 		= 0xfe;
const           EMPTY 		= 0xff;
const           MAXKER 		= 64;		/* maximum kernel size */
const           MAXSTR 		= 256;		/* maximum string length */
const           MAXPIX 		= 1036800;	/* handle 1152x900 pixel regions */

/*
 * Define stuctures shared by client and server.
 */
struct Image {				/* (sub)image, byte-deep pixels */
  string          sName < MAXSTR >;	/* miscellaneous NULL-term'd strings */
  int             x;
  int             y;			/* tl corner of the pixel region */
  int             dx;
  int             dy;			/* region width and height */
  opaque          Data < MAXPIX >;	/* the raw data */
  struct Image   *pNext;		/* requires system w/ xdr_pointer() support */
};

struct Kernel {
  string          sName < MAXSTR >;
  int             xsize;
  int             ysize;
  float           Value < MAXKER >;
  struct Kernel  *pNext;
};

/*
 * Define the client and server data exchanged.  Be consistent here. It's
 * important to fully define union structure elements at run time - leave a
 * dangling field or access a bogus one, and get very erratic behavior.
 * xdr_pointer() is handy but unforgiving.
 */
union Packet switch (int op) {
  case ADDSERVER:	string sOp < MAXSTR >;
  case READIMAGE:	Image * pImage;
  case READKERNEL:	Kernel * pKernel;
  case CONVOLVE:	Image * pImage;
  case SUCCESS:		void;
  case ERROR:		string sOp < MAXSTR >;
  default: 		string sOp < MAXSTR >;
};

/*
 * The rip server procedure definition.
 */
program RIPPROG {
  version RIPVERS {
    Packet RIP(Packet) = 1;
  } = 1;
} =  0x20000001;
