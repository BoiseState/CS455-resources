/*
 * Just to be consistent, define all the shared integer constants here.
 */
const ERROR = 0x0;	/* status codes used for exiting children */
const SUCCESS = 0x1;

const MAXSTR = 256;	/* maximum string length */
const MAXU = 1024;	/* maximum columns in the bkgnd. pat */
const MAXV = 1024;	/* maximum lines in the bkgnd. pat */
const MAXBALLS = 1024;  /* guess */

/* Loading limits */
const DEFLOAD = 25;	/* default starting server loading in scan lines */
const MINLOAD = 15;	/* minimum load */
const MAXLOAD = 35;
const MAXPIX = 88200;	/* handle one SCALExMAXLOADx(XMAX-XMIN) region */
const LEVEL = 6;	/* levels of recursion */
const RLEV = 5;		/* don't want as many inside the ball, takes
			 * forever as it is */

/* process control times */
const STIME = 1;	/* how long to sleep between forks */
const REXETIME = 20;	/* how long to wait for rexec's to start */

/*
 * Define stuctures used by client and server.
 */
struct color {
  int             r;
  int             g;
  int             b;
};

struct vector {
  double          x;
  double          y;
  double          z;
  double          l;
  double          xzl;
};

struct ray {
  struct vector   org;
  struct vector   dir;
};

struct sphere {
  struct vector   cent;
  double          rad;
};

struct parameters {
  struct sphere  *ls;
  struct vector  *vp;
  double         *bkcon;
};

struct ball {
  struct sphere   s;
  double          ior;
  double          rfr;
  double          rfl;
  double          dif;
  double          amb;
};

struct mat {
  struct vector   x;		/* first !row! */
  struct vector   y;		/* second !row! */
  struct vector   z;
};				/* third !row! */

/*
 * attempts at sending raw pixel data to clients yields the following times:
 * 17, 42, 47, 180, 43, 25 seconds of init times per server! Thus passing
 * file name is opted for. These init times are: 17, 27, 30, 37, 23, 13
 * seconds - 2.4X faster!
 */
typedef string  filename < MAXSTR >;

typedef ball    balls < MAXBALLS >;

struct lines {			/* start and stop assignments for servers */
  double          ymin;
  double          ymax;
};


/*
 * Define the client and server data exchanged.  Be consistent here. It's
 * important to fully define union structure elements at run time - leave a
 * dangling field or access a bogus one, and get very erratic behavior.
 * xdr_pointer() is handy but unforgiving.
 */
union reply switch (int op) {
  case SUCCESS:void;
  case ERROR:
    string sOp < MAXSTR >;
  default:
    string sOp < MAXSTR >;
};

typedef char    replypix < MAXPIX >;

/*
 * The rtrace server procedure definition.
 */
program         RTRACESERVER {
  version         RTRACEVERS {
    void            DIE(void) = 1;
    void            RESTART(void) = 2;
    reply           SET_PARAMS(parameters) = 3;
    reply           LOAD_BKGND(filename) = 4;
    reply           LOAD_BALLS(balls) = 5;
    replypix        RAY_TRACE(lines) = 6;
  } =             1;
} =             0x20000001;
