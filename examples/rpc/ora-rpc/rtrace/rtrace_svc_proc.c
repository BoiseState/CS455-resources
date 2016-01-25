/*
 * rtrace_svc_proc.c: rtrace remote server procedures
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/param.h>
#include <rpc/rpc.h>
#include "rtrace.h"
#include "rtrace_shared.h"

reply           Reply;
char 		*malloc();

/*
 * Ray tracing stuff.
 */
double          bkgnd[MAXU][MAXV];
double          bkcon;
ball           *bl = NULL;
sphere          ls;
vector          vp;
int             level, nob;
int             maxu, maxv;

void           *
die_1()
{
  /*
   * Unregister the server and die gracefully, returning
   * nothing.
   */
  svc_unregister(RTRACESERVER, RTRACEVERS);
  exit((int)SUCCESS);
}

void           *
restart_1()
{
  /*
   * Unregister the server and start up again, returning
   * nothing.
   */
  svc_unregister(RTRACESERVER, RTRACEVERS);
  (void) execl("/home/cortex/u0/bloomer/src/rpc/rtrace/xray/rtrace_svc",
	(char *) 0);
}

reply          *
set_params_1(pparams)
  parameters     *pparams;
{
  /*
   * Establish scene-specific rendering parameters.
   */
  printf("set_params_1\n");
  bcopy((char *)pparams->ls, (char *)&ls, sizeof(vector));
  bcopy((char *)pparams->vp, (char *)&vp, sizeof(vector));
  bkcon = *(pparams->bkcon);
  Reply.op = SUCCESS;
  return (&Reply);
}

reply          *
load_bkgnd_1(fname)
  filename       *fname;
{
/*
 * Server procedure to load a database. As
 * the protocol suggests, Request Packets are returned with
 * some error logging.
 */
  int             u, v;
  u_char          pval, big = 0, little = 255;
  colormap_t      cm;
  rasterfile      rast;
  u_char         *d;
  FILE           *fp;

  printf("load_bkgnd_1\n");
  Reply.op = ERROR;

  /* Read in a Sun rasterfile and make some room */
  fp = fopen(*fname, "r");
  if (!fp || !(d = (u_char *) ReadSunRaster(fp, &cm, &rast))) {
    WARNMSG("can't load background raster file:");
    WARNMSG(*fname);
    Reply.reply_u.sOp = strdup("can't load background raster file");
    return (&Reply);
  }
  if (((maxu = rast.ras_width) > MAXU) ||
      ((maxv = rast.ras_height) > MAXV)) {
    WARNMSG("background pattern too big");
    Reply.reply_u.sOp = strdup("background pattern too big");
    return (&Reply);
  }
  /* Normalize and stuff into a double array */
  for (v = 0; v < maxv; v++)
    for (u = 0; u < maxu; u++) {
      pval = (u_char) * (d + v * maxu + u);
      little = MIN(pval, little);
      big = MAX(pval, big);
    }
  big = big - little;
  for (v = 0; v < maxv; v++)
    for (u = 0; u < maxu; u++) {
      pval = (u_char) * (d + v * maxu + u);
      bkgnd[u][v] = (double) (pval - little) / (double) big;
    }

  /* Build up Reply packet */
  Reply.op = SUCCESS;
  return (&Reply);
}

reply          *
load_balls_1(pBalls)
  balls          *pBalls;
{
  /*
   * Load the balls.
   */
  printf("load_balls_1\n");
  Reply.op = ERROR;
  nob = pBalls->balls_len;
  if (bl)
    free((char *)bl);
  bl = (ball *) malloc(nob*sizeof(ball));
  bcopy((char *)pBalls->balls_val, (char *)bl, nob*sizeof(ball));

  Reply.op = SUCCESS;
  return (&Reply);
}

replypix       *
ray_trace_1(pLines)
  lines          *pLines;
{
  /*
   * Actually do the ray tracing.  Generate and cast rays,
   * assigning a grey value for each point in the viewplane (pixel).
   */
  ray             rr;
  static replypix rp;
  static char    *data = NULL;
  char           *p;
  static double   xmin, xmax, ymin, ymax;
  double          xco, yco;

  /* Allocate some static return space */
  if (!data) {
    xmin = XMIN * SCALE;
    xmax = XMAX * SCALE;
    ymin = YMIN * SCALE;
    ymax = YMAX * SCALE;
    data = malloc((unsigned)nint((xmax - xmin) * (ymax - ymin)));
  }
  /* Cast rays for the assigned scan lines. */
  p = data;
  ymin = pLines->ymin;
  ymax = pLines->ymax;
  for (yco = ymax; yco > ymin; yco--) {
    printf("Scan-line %d\n", (int) yco);
    for (xco = xmin; xco < xmax; xco++) {
      MV(xco / SCALE, yco / SCALE, 0.0, rr.org);
      SV(rr.dir, rr.org, vp);
      *(p++) = (char) shade(&rr);	/* trace them */
    }
  }
  rp.replypix_len = p - data;
  rp.replypix_val = data;
  return (&rp);
}

int 
shade(r)
  ray            *r;
{
  /*
   * This function does all the real work- calculating the
   * shade each pixel should be. It uses recursion.
   */
  int             i, c, refract();
  ray             refr;
  double          lght, z, l, k, dot(), find(),
                  Shadow();
  int             sx, sy;
  double          stupid;
  vector          new, norm;
  mat             trans;
  sphere          ss;
  ball           *b;

  if (++level <= LEVEL) {
    c = -1;
    l = HUGE;
    /* get vector length and xz component for mt() */
    r->dir.l = LEN(r->dir);
    r->dir.xzl = XZL(r->dir);
    /*
     * make a transform matrix that rotates something in
     * space so that the ray will be aligned with the x
     * axis
     */
    mt(&(r->dir), &trans);

    /* for starters we find out whether we hit anything. */
    for (i = 0; i < nob; i++) {
      b = bl + i;		/* ball pointer */
      ss.rad = b->s.rad;
      SV(ss.cent, b->s.cent, r->org);
      if ((k = find(&trans, &ss)) > 0.0 && k < l) {
	c = i;
	l = k;
      }
    }

    if (c >= 0 && (l * trans.x.y + r->org.y) > 0.0) {
      /* WE HIT SOMETHING */
      MV(l * trans.x.x, l * trans.x.y, l * trans.x.z, new);
      new.l = l;
      /* move the new orgin of the ray to the intersection */
      AV(refr.org, new, r->org);
      AV(r->org, new, r->org);
      MV(r->dir.x, r->dir.y, r->dir.z, refr.dir);
      /* get a normal vector for the intersection point */
      b = bl + c;
      SV(norm, r->org, b->s.cent);
      norm.l = b->s.rad;

      /* ambient lighting */
      lght = 200.0 * b->amb;

      /*
       * shaded lighting (diffuse). subroutine Shadow is in
       * find.c
       */
      if (b->dif != 0.0) {
	SV(new, ls.cent, r->org);
	new.l = LEN(new);
	if ((k = DOT(new, norm)) > 0.0)
	  lght += b->dif * Shadow(&(r->org)) * k / (new.l) / (norm.l);
      }
      /* reflection... easy */
      if (b->rfl != 0.0) {
	/* make the normal unit length */
	SCMLT((1.0 / norm.l), norm);
	/*
	 * get the length of the ray's component in the
	 * normal direction
	 */
	stupid = 2.0 * DOT(norm, r->dir);
	SCMLT(stupid, norm);
	/*
	 * subtract double the normal component-
	 * !reflection!
	 */
	SV(r->dir, r->dir, norm);
	lght += b->rfl * (double) shade(r);
      }
      /*
       * refraction. this is ugly, which is why I choose to
       * deal with it in it's own subroutine which comes
       * after this one
       */
      if (b->rfr != 0.0) {
	lght += b->rfr * (double) refract(&refr, b);
      }
    } else {			/* hit no objects... */
      if ((r->dir.y) < 0.0) {	/* crosses floor */
	z = -(r->org.y) / (r->dir.y);
	(r->org.x) += z * (r->dir.x);
	(r->org.z) += z * (r->dir.z);
	(r->org.y) = 0.0;

	SV(new, ls.cent, r->org);
	new.l = LEN(new);
	sx = (int) (r->org.x / 1.5) % maxu;
	if (sx < 0)
	  sx += maxu;
	sy = -(int) (r->org.z / 1.5) % maxv;
	if (sy < 0)
	  sy += maxv;
	lght = (bkcon * bkgnd[sx][sy] + 1.0 - bkcon) * (0.8 *
	    Shadow(&(r->org)) * (new.y) / (new.l) + 40.0);


      } else {			/* check to see if it hit
				 * lightsource */
	SV(ss.cent, ls.cent, r->org);
	ss.rad = ls.rad;
	if (find((mat *)&trans, (sphere *)&(ss.cent)) > 0.0)
	  lght = 255;
	else
	  lght = 0;
      }
    }
  }
  /* to many levels return 0 cause it shouldn't matter */
  else
    lght = 0;
  level--;
  if (lght < 0.0)
    lght = 0.0;
  if (lght > 255.0)
    lght = 255.0;
  return ((int) lght);
}

int             rlev;
int 
refract(r, bll)
  ray            *r;
  ball           *bll;
{
  vector          norm;
  ray             ir;
  double          refk(), getcapt(), capt, inside();
  double          stupid;

  SV(norm, r->org, bll->s.cent);
  norm.l = bll->s.rad;

  capt = getcapt(&norm, &(r->dir), bll->ior);


  /* get the addition factor for the normal for refraction */
  stupid = refk(&(norm), &(r->dir), bll->ior);
  SCMLT(stupid, norm);

  AV(ir.dir, r->dir, norm);
  MV(r->org.x, r->org.y, r->org.z, ir.org);

  /* now get it for reflection */
  SV(norm, r->org, bll->s.cent);
  norm.l = bll->s.rad;
  SCMLT(1.0 / norm.l, norm);
  stupid = 2.0 * DOT(norm, r->dir);
  SCMLT(stupid, norm);
  SV(r->dir, r->dir, norm);

  return ((int) ((1.0 - capt) * (double) shade(r) + ((capt) * inside(&ir, bll))));
}

double 
inside(r, bll)
  ray            *r;
  ball           *bll;
{
  vector          new, norm;
  mat             trans;
  ray             er;
  double          findo(), lght, l, refk(), getcapt(),
                  capt;
  double          stupid;
  sphere          ss;


  if (++rlev < RLEV) {
    r->dir.l = LEN(r->dir);
    r->dir.xzl = XZL(r->dir);
    mt(&(r->dir), &trans);
    ss.rad = bll->s.rad;
    SV(ss.cent, bll->s.cent, r->org);

    l = findo(&trans, &ss);
    MV(l * trans.x.x, l * trans.x.y, l * trans.x.z, new);
    AV(er.org, r->org, new);
    AV(r->org, r->org, new);
    SV(norm, er.org, bll->s.cent);

    norm.l = bll->s.rad;
    capt = getcapt(&norm, &(r->dir), 1.0 / bll->ior);

    stupid = refk(&norm, &(r->dir), 1.0 / bll->ior);
    SCMLT(stupid, norm);
    AV(er.dir, norm, r->dir);

    SCMLT(1.0 / norm.l, norm);
    stupid = 2.0 * DOT(norm, r->dir);
    SCMLT(stupid, norm);
    SV(r->dir, r->dir, norm);
    lght = (1.0 - capt) * inside(r, bll) + (capt * (double) shade(&er));
  } else
    lght = 0.0;
  rlev--;
  if (lght < 0.0)
    lght = 0.0;
  if (lght > 255.0)
    lght = 255.0;
  return (lght);
}

double 
refk(nrm, in, ior)
  vector         *nrm, *in;
  double          ior;
{
  double          dt, ln, li, ret;

  ior = ior * ior;
  dt = DOT((*nrm), (*in));
  ln = LN2((*nrm));
  li = LN2((*in));
  if (dt < 0)
    ret = (-dt - sqrt(dt * dt - ln * li * (1 - ior))) / ln;
  else
    ret = (-dt + sqrt(dt * dt - ln * li * (1 - ior))) / ln;
  return (ret);
}

double 
getcapt(nrm, dr, ior)
  vector         *nrm, *dr;
  double          ior;
{
  double          dt, cs1, cs2, p, s;
  dt = DOT((*nrm), (*dr));
  dt = dt * dt / LN2((*nrm)) / LN2((*dr));
  cs1 = sqrt(dt);
  cs2 = sqrt(1.0 - (1.0 - dt) / ior);
  p = cs1 / (cs1 + ior * cs2);
  s = cs1 / (ior * cs1 + cs2);
  return (2.0 * (p * p + s * s));
}

double 
findo(m, s)			/* finds where a ray inside
				 * the ball exits. */
  mat            *m;
  sphere         *s;
{
  /* foops id the rotated position vector. */
  vector          foops;
  double          t;
  MTV(foops, (*m), s->cent);
  /* see if it hits the ball (it better) */
  t = s->rad * s->rad - foops.y * foops.y - foops.z * foops.z;
  if (t > 0)
    t = foops.x + sqrt(t);
  else
    t = 0;
  /* return how far along the ray you were when you hit */
  return (t);
}

double 
find(m, s)			/* finds whether a ray hits
				 * a ball */
  mat            *m;
  sphere         *s;
{
  vector          foops;
  double          t;
  MTV(foops, (*m), s->cent);
  t = s->rad * s->rad - foops.y * foops.y - foops.z * foops.z;
  if (t > 0)
    t = foops.x - sqrt(t);
  else
    t = 0;
  return (t);
}

double 
finds(m, s)			/* finds if a ball is
				 * between a point and a
				 * lightsource. Returns how
				 * obscuring the ball is */
  mat            *m;
  sphere         *s;
{
  vector          foops;
  double          t;
  MTV(foops, (*m), s->cent);
  t = s->rad - sqrt(foops.y * foops.y + foops.z * foops.z);
  if (t > 0)
    t = t / foops.x;
  else
    t = 0;
  return (t);
}

double 
Shadow(p)			/* finds if a point is in a
				 * Shadow, or if it is on
				 * edge */
  vector         *p;
{
  mat             trans;
  sphere          ss;
  vector          d;
  int             c, i;
  double          l, k, finds();
  ball           *b;

  l = 0.0;
  c = -1;
  SV(d, ls.cent, (*p));
  d.l = LEN(d);
  d.xzl = XZL(d);
  mt(&(d), &trans);

  for (i = 0; i < nob; i++) {
    b = bl + i;
    ss.rad = b->s.rad;
    SV(ss.cent, b->s.cent, (*p));
    if ((k = finds(&trans, &ss)) > l) {
      c = i;
      l = k;
    }
  }
  if (c == -1)
    k = 200.0;
  else {
    k = 1.0 - l / ((ls.rad) / (d.l));
    if (k < 0.0)
      k = 0.0;
    k *= 200.0;
  }
  return (k);
}

/*
 * supportive subroutines...
 */

mt(vec, trans)
  vector         *vec;
  mat            *trans;
{
  if (vec->xzl == 0.0) {
    trans->x.x = 0.0;
    trans->x.y = 1.0;
    trans->x.z = 0.0;
    trans->y.x = -1.0;
    trans->y.y = 0.0;
    trans->y.z = 0.0;
    trans->z.x = 0.0;
    trans->z.y = 0.0;
    trans->z.z = 1.0;
  } else {
    trans->x.x = (vec->x) / (vec->l);
    trans->x.y = (vec->y) / (vec->l);
    trans->x.z = (vec->z) / (vec->l);
    trans->y.x = -(vec->x) * (vec->y) / ((vec->l) * (vec->xzl));
    trans->y.y = (vec->xzl) / (vec->l);
    trans->y.z = -(vec->z) * (vec->y) / ((vec->l) * (vec->xzl));
    trans->z.x = -(vec->z) / (vec->xzl);
    trans->z.y = 0;
    trans->z.z = (vec->x) / (vec->xzl);
  }
}
