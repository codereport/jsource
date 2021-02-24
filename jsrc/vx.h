/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Extended Precision                                                      */

#define XIDIG 5 /* max # x digits in an integer */

#define XBASE (I)10000
#define XBASEN (I)4
#define XPINF (I)99999
#define XNINF (I) - 99999
#define XDIG(a) (AV(a)[AN(a) - 1]) /* leading digit              */
#define XMAX(x, y) (1 == jtxcompare(jt, x, y) ? x : y)
#define XMIN(x, y) (1 == jtxcompare(jt, x, y) ? y : x)
#define XDIV(x, y) jtxdiv(jt, x, y, jt->xmode)
#define XCUBE(x) jtxtymes(jt, x, jtxtymes(jt, x, x))

/* values for jt->xmode */

#define XMFLR 0   /* floor,   round down        */
#define XMCEIL 1  /* ceiling, round up          */
#define XMEXACT 2 /* exact, error if impossible */
#define XMEXMT 3  /* exact, empty if impossible */

extern X jtxsgn(J jt, X w);
extern X jtxsq(J jt, X w);
extern X jtxsqrt(J jt, X w);
extern X jtxstd(J jt, X w);
extern X jtxgcd(J jt, X a, X w);
extern X jtxlcm(J jt, X a, X w);
extern X jtxminus(J jt, X a, X w);
extern X jtxplus(J jt, X a, X w);
extern X jtxpow(J jt, X a, X w);
extern X jtxtymes(J jt, X a, X w);

extern X jtxc(J, I);
extern I jtxcompare(J, X, X);
extern X jtxdiv(J, X, X, I);
extern B jtxdivrem(J, X, X, X*, X*);
extern X jtxev1(J, A, C*);
extern X jtxev2(J, A, A, C*);
extern I jtxint(J, X);
extern D jtxlogabs(J, X);
extern X jtxrem(J, X, X);
