/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Complex Numbers                                                         */

#define THMAX 5.9631365372958016e8
#define EMAX 709.78271289338409
#define EMIN -744.44007192138126
#define EMAX2 710.47586007394398
#define TMAX 19.066172834610153

/* THMAX:  2p1*2^b%2 where b is # bits in significand   */
/* EMAX:   largest  number where _>^EMAX;  1024*^.2     */
/* EMIN:   smallest number where 0<^EMIN; _1074*^.2     */
/* EMAX2:  largest  number where _>cosh EMAX2; EMAX+^.2 */
/* TMAX:   smallest number where 1=tanh TMAX            */

#define OFLOMAX 1e306   // larger than this must be reduced to avoid oflo in hypoth
#define UFLOMIN 1e-305  // smaller than this must be increased to avoid uflo in hypoth

#define ZF1DECL \
    Z z;        \
    D zr = 0.0, zi = 0.0, a = v.re, b = v.im
#define ZF2DECL \
    Z z;        \
    D zr = 0.0, zi = 0.0, a = u.re, b = u.im, c = v.re, d = v.im
#define ZEPILOG \
    z.re = zr;  \
    z.im = zi;  \
    return z
#define ZS1(f, stmts) \
    Z f(J jt, Z v) {  \
        ZF1DECL;      \
        stmts;        \
        ZEPILOG;      \
    }
#define ZS2(f, stmts)     \
    Z f(J jt, Z u, Z v) { \
        ZF2DECL;          \
        stmts;            \
        ZEPILOG;          \
    }
#define MMM(a, b)    \
    {                \
        p = ABS(a);  \
        q = ABS(b);  \
        if (p < q) { \
            D t = p; \
            p   = q; \
            q   = t; \
        }            \
    }

#define ZASSERT(b, e)         \
    {                         \
        if (!(b)) {           \
            jtjsignal(jt, e); \
            return zeroZ;     \
        }                     \
    }
#define ZNZ(v) ((v).re || (v).im)
#define ZEZ(v) (!((v).re || (v).im))
#define ZINF(v) (inf == (v).re || inf == (v).im || infm == (v).re || infm == (v).im)
#define ZCJ(u, v) ((u).re == (v).re && (u).im == -(v).im)
#define ZOV(v) (ABS((v).re) > OFLOMAX || ABS((v).im) > OFLOMAX)
#define ZUN(v) (ABS((v).re) < UFLOMIN || ABS((v).im) < UFLOMIN)
#define ZRE(x, y) (TYMES((x).re, (y).re) - TYMES((x).im, (y).im))
#define ZIM(x, y) (TYMES((x).re, (y).im) + TYMES((x).im, (y).re))

extern Z jtzceil(J jt, Z v);
extern Z jtzconjug(J jt, Z v);
extern Z jtzexp(J jt, Z v);
extern Z jtzlog(J jt, Z v);
extern Z jtznegate(J jt, Z v);
extern Z jtzsqrt(J jt, Z v);
extern Z jtztrend(J jt, Z v);
extern Z jtzfloor(J jt, Z v);

extern Z jtzcir(J jt, Z u, Z v);
extern Z jtzdiv(J jt, Z u, Z v);
extern Z jtzgcd(J jt, Z u, Z v);
extern Z jtzlcm(J jt, Z u, Z v);
extern Z jtzminus(J jt, Z u, Z v);
extern Z jtzplus(J jt, Z u, Z v);
extern Z jtzpow(J jt, Z u, Z v);
extern Z jtzrem(J jt, Z u, Z v);
extern Z jtztymes(J jt, Z u, Z v);

extern B jtzeq(J, Z, Z);
extern B jtztridiag(J, I, A, A);

extern D zmag(Z);
extern Z zrj0(D);
