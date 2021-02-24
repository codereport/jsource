/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Gaussian Elimination                                             */

#include "j.h"

// w is a rational matrix
A
jtgausselm(J jt, A w) {
    I c, e, i, j, r, r1, *s;
    Q p, *u, *v, *x;
    F1RANK(2, jtgausselm, UNUSED_VALUE);
    ASSERT(RAT & AT(w), EVNONCE);
    // This routine modifies w in place.  If w is virtual, that causes an error, because the blocks referred to in
    // w are actually in the backer, and the backer has had ra() applied; so blocks in the backer are going to be freed
    // twice: storing a newly-allocated block will produce a double-free.  So we have to realize any virtual block
    // coming in.
    ASSERT(2 == AR(w), EVRANK);
    s      = AS(w);
    r      = s[0];
    c      = s[1];
    r1     = MIN(r, c);
    A *old = jt->tnextpushp;
    for (j = 0; j < r1; ++j) {
        v = QAV(w);
        e = -1;
        u = v + c * j + j;
        DO(
          r - j, if (XDIG(u->n)) {
              e = i + j;
              break;
          } u += c;); /* find pivot row */
        ASSERT(0 <= e, EVDOMAIN);
        x = v + c * j;
        if (j != e) {
            u = v + c * e;
            DO(c, Q t1 = u[i]; u[i] = x[i]; x[i] = t1;);
        } /* interchange rows e and j */
        p = x[j];
        ra(p.n);
        ra(p.d);
        DO(c, Q z = jtqdiv(jt, x[i], p); INSTALLRAT(w, x, i, z););
        fa(p.n);
        fa(p.d);
        for (i = 0; i < r; ++i) {
            if (i == j) continue;
            u = v + c * i;
            p = u[j]; /* pivot */
            ra(p.n);
            ra(p.d);
            DO(c, Q z = jtqminus(jt, u[i], jtqtymes(jt, p, x[i])); INSTALLRAT(w, u, i, z););
            fa(p.n);
            fa(p.d);
        }
        if (!jtgc3(jt, &w, 0L, 0L, old))
            return 0;  // use simple gc3 to ensure all changes use the stack, since w is modified inplace. Alternatively
                       // could turn off inplacing here
    }
    return w;
} /* Gaussian elimination in place */

static A
jtdetr(J jt, A w) {
    A z;
    I c, e, g = 1, i, j, k, r, *s;
    Q d, p, *u, *v, *x;
    s      = AS(w);
    r      = s[0];
    c      = s[1];
    A *old = jt->tnextpushp;
    for (j = 0; j < r; ++j) {
        v = QAV(w);
        e = -1;
        u = v + c * j + j;
        DO(
          r - j, if (XDIG(u->n)) {
              e = i + j;
              break;
          } u += c;); /* find pivot row */
        if (0 > e) return jtcvt(jt, RAT, num(0));
        x = v + c * j;
        if (j != e) {
            u = v + c * e;
            DO(c, Q t1 = u[i]; u[i] = x[i]; x[i] = t1;);
            g = -g;
        } /* interchange rows e and j */
        i = XDIG(x[j].n);
        if (i == XPINF || i == XNINF) return mark;
        for (i = j + 1; i < r; ++i) {
            u = v + c * i;
            if (XDIG(u[j].n)) {
                p = jtqdiv(jt, u[j], x[j]);
                ra(p.n);
                ra(p.d);
                for (k = j + 1; k < r; ++k) {
                    Q z = jtqminus(jt, u[k], jtqtymes(jt, p, x[k]));
                    INSTALLRAT(w, u, k, z);
                }
                fa(p.n);
                fa(p.d);
            }
        }
        if (!jtgc3(jt, &w, 0L, 0L, old))
            return 0;  // use simple gc3 to ensure all changes use the stack, since w is modified inplace. Alternatively
                       // could turn off inplacing here
    }
    d = 0 < g ? *v : jtqminus(jt, zeroQ, *v);
    u = v + 1 + c;
    DQ(r - 1, d = jtqtymes(jt, d, *u); u += 1 + c;);
    RE(0);
    GAT0(z, RAT, 1, 0);
    QAV(z)[0] = d;
    return z;
} /* determinant on rational matrix; works in place */

static A
jtdetd(J jt, A w) {
    D g, h, p, q, *u, *v, *x, *y, z = 1.0;
    I c, d, e, i, j, k, r, *s;
    s = AS(w);
    r = s[0];
    c = s[1];
    v = DAV(w);
    NAN0;
    for (j = 0; j < r; ++j) {
        x = v + c * j;
        u = x + j;
        h = 0.0;
        DO(r - j, k = i; DO(
             c - j, g = ABS(*u); if (h < g) {
                 h = g;
                 d = j + k;
                 e = j + i;
             } ++u;);
           u += j;); /* find pivot, maximum abs element */
        if (h == inf) return mark;
        if (0 == h) return jtscf(jt, 0.0);
        if (j != d) {
            u = v + c * d + j;
            y = x + j;
            DQ(c - j, q = *u; *u = *y; *y = q; ++u; ++y;);
            z = -z;
        } /* interchange rows j and d */
        if (j != e) {
            u = x + e;
            y = x + j;
            DQ(r - j, q = *u; *u = *y; *y = q; u += c; y += c;);
            z = -z;
        } /* interchange cols j and e */
        q = x[j];
        z *= q;
        JBREAK0;
        for (i = j + 1; i < r; ++i) {
            u = v + c * i;
            if (u[j]) {
                p = u[j] / q;
                for (k = j + 1; k < r; ++k) u[k] -= p * x[k];
            }
        }
    }
    NAN1;
    return jtscf(jt, z);
} /* determinant on real     matrix; works in place */

#define ZABT(v) ((v).re * (v).re + (v).im * (v).im)

static A
jtdetz(J jt, A w) {
    A t;
    D g, h;
    I c, d, e, i, j, k, r, *s;
    Z p, q, *u, *v, *x, *y, z;
    z.re = 1.0;
    z.im = 0.0;
    s    = AS(w);
    r    = s[0];
    c    = s[1];
    v    = ZAV(w);
    NAN0;
    for (j = 0; j < r; ++j) {
        x = v + c * j;
        u = x + j;
        h = 0.0;
        DO(r - j, k = i; DO(
             c - j, g = ZABT(*u); if (h < g) {
                 h = g;
                 d = j + k;
                 e = j + i;
             } ++u;);
           u += j;); /* find pivot, maximum abs element */
        if (h == inf) return mark;
        if (0 == h) return jtscf(jt, 0.0);
        if (j != d) {
            u = v + c * d;
            DO(c - j, q = u[j + i]; u[j + i] = x[j + i]; x[j + i] = q;);
            z = jtzminus(jt, zeroZ, z);
        } /* interchange rows j and d */
        if (j != e) {
            u = v + c * j + e;
            y = v + c * j + j;
            DQ(r - j, q = *u; *u = *y; *y = q; u += c; y += c;);
            z = jtzminus(jt, zeroZ, z);
        } /* interchange cols j and e */
        q = x[j];
        z = jtztymes(jt, z, q);
        for (i = j + 1; i < r; ++i) {
            u = v + c * i;
            if (ZNZ(u[j])) {
                p = jtzdiv(jt, u[j], q);
                for (k = j + 1; k < r; ++k) u[k] = jtzminus(jt, u[k], jtztymes(jt, p, x[k]));
            }
        }
    }
    NAN1;
    RE(0);
    GAT0(t, CMPX, 1, 0);
    ZAV(t)[0] = z;
    return t;
} /* determinant on complex  matrix; works in place */

A
jtgaussdet(J jt, A w) {
    A z;
    I *s;
    ASSERT(2 == AR(w), EVRANK);
    s = AS(w);
    ASSERT(s[0] == s[1], EVLENGTH);
    switch (CTTZNOFLAG(AT(w))) {
        default: ASSERT(0, EVDOMAIN);
        case B01X:
        case INTX: return jtdetd(jt, jtcvt(jt, FL, w));
        case FLX: z = jtdetd(jt, jtca(jt, w)); break;
        case CMPXX: z = jtdetz(jt, jtca(jt, w)); break;
        case XNUMX: z = jtdetr(jt, jtcvt(jt, RAT, w)); break;
        case RATX: z = jtdetr(jt, jtca(jt, w));
    }
    return z == mark ? jtdetxm(jt, w, jteval(jt, "-/ .*")) : z;
} /* determinant on square matrix */
