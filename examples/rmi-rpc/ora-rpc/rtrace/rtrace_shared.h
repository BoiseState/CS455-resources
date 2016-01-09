#include "../X11/Compare/raster.h"

/* window dimensions,  in virtual units */
#define XMIN 10.0
#define XMAX 220.0
#define YMIN 10.0
#define YMAX 170.0
#define SCALE 4.0		/* magnification factor */

#define MAXSERV sizeof(int)*8
#ifdef HUGE
#undef HUGE
#endif
#define HUGE 99999999999.0

#define ERRMSG(str)	{ printf ("%s\n", str); exit (-1); }
#define WARNMSG(str)	{ printf ("%s\n", str); }

/* some of the most important stuff in the program */
#define DOT(v1,v2) (v1.x*v2.x+v1.y*v2.y+v1.z*v2.z)
/* returns dot product of two vectors */
#define LN2(v)	   (DOT(v,v))
/* returns the square of the length of a vector */
#define LEN(v)	   sqrt(LN2(v))
/* guess */
#define XZL(v)	   sqrt(v.x*v.x+v.z*v.z)
/* returns the component in the xz plane of a vector */
#define SCMLT(sc,vct) vct.x*= sc;vct.y*= sc;vct.z*= sc;vct.l*= sc;
/* multiplies a vetor by a scalar */
#define MV(a,b,c,v)   v.x= a;v.y= b;v.z= c;
/* makes a vector. wouldn't need this with c++ */
#define SV(t,u,v)  t.x=u.x-v.x;t.y=u.y-v.y;t.z=u.z-v.z;
/* subtract vector t=u-v */
#define AV(t,u,v)  t.x=u.x+v.x;t.y=u.y+v.y;t.z=u.z+v.z;
/* add vector t=u+v */
#define MTV(v1,m,v2) MV(DOT(m.x,v2),DOT(m.y,v2),DOT(m.z,v2),v1)
/* multiply transpose matrix by vector. v1=m*v2 */
