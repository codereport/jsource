/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Rational Numbers                                                 */

#include "j.h"
#include "ve.h"

Q
jtqstd(J jt, Q w) {
    I d, n;
    Q z;
    X g;
    QRZ(w.n && w.d && !jt->jerr);
    n   = XDIG(w.n);
    d   = XDIG(w.d);
    z.d = iv1;
    if (0 > d) {
        QRE(w.n = jtrifvs(jt, jtnegate(jt, w.n)));
        QRE(w.d = jtrifvs(jt, jtnegate(jt, w.d)));
        n = -n;
        d = -d;
    }  // w could become the result
    if (!n) {
        z.n = iv0;
        return z;
    }
    if (!d) {
        z.n = jtvci(jt, 0 < n ? XPINF : XNINF);
        return z;
    }
    if (d == XPINF) {
        QASSERT(n != XPINF && n != XNINF, EVNAN);
        return zeroQ;
    }
    if (n == XPINF || n == XNINF) {
        z.n = w.n;
        return z;
    }
    QRE(g = jtxgcd(jt, w.n, w.d));
    if (QX1(g)) return w;
    z.n = xdiv(w.n, g, XMEXACT);
    z.d = xdiv(w.d, g, XMEXACT);
    return z;
}

Q
jtqplus(J jt, Q a, Q w) {
    PROLOG(0083);
    Q z;
    z.n = jtxplus(jt, jtxtymes(jt, a.n, w.d), jtxtymes(jt, w.n, a.d));
    z.d = jtxtymes(jt, a.d, w.d);
    QEPILOG(z);
}

Q
jtqminus(J jt, Q a, Q w) {
    PROLOG(0084);
    Q z;
    z.n = jtxminus(jt, jtxtymes(jt, a.n, w.d), jtxtymes(jt, w.n, a.d));
    z.d = jtxtymes(jt, a.d, w.d);
    QEPILOG(z);
}

Q
jtqtymes(J jt, Q a, Q w) {
    PROLOG(0085);
    Q z;
    z.n = jtxtymes(jt, a.n, w.n);
    z.d = jtxtymes(jt, a.d, w.d);
    QEPILOG(z);
}

Q
jtqdiv(J jt, Q a, Q w) {
    PROLOG(0086);
    Q z;
    z.n = jtxtymes(jt, a.n, w.d);
    z.d = jtxtymes(jt, a.d, w.n);
    QEPILOG(z);
}

static Q
jtqrem(J jt, Q a, Q w) {
    PROLOG(0087);
    I c, d;
    Q m, q, z;
    c = XDIG(a.n);
    d = XDIG(w.n);
    if (!c) return w;
    QASSERT(!(d == XPINF || d == XNINF), EVNAN);
    if (c == XPINF) return 0 <= d ? w : a;
    if (c == XNINF) return 0 >= d ? w : a;
    q   = jtqdiv(jt, w, a);
    m.n = jtxtymes(jt, a.n, xdiv(q.n, q.d, XMFLR));
    m.d = a.d;
    z   = jtqminus(jt, w, m);
    QEPILOG(z);
}

static Q
jtqgcd(J jt, Q a, Q w) {
    PROLOG(0088);
    Q z;
    QRE(z.n = jtxgcd(jt, a.n, w.n));
    QRE(z.d = jtxlcm(jt, a.d, w.d));
    QEPILOG(z);
}

static Q
jtqlcm(J jt, Q a, Q w) {
    return jtqtymes(jt, a, jtqdiv(jt, w, jtqgcd(jt, a, w)));
}

static Q
jtqpow(J jt, Q a, Q w) {
    PROLOG(0089);
    B c;
    I p, q, s;
    Q t, z;
    X d;
    QRE(1);
    t   = a;
    d   = w.n;
    z.n = z.d = iv1;
    p         = XDIG(a.n);
    q         = XDIG(w.n);
    c         = QX1(w.d);
    if (p == XPINF || p == XNINF) {
        QASSERT(0 < p || q != XPINF, EVDOMAIN);
        z.n = jtrifvs(jt, jtvci(jt, !q ? 1L : 0 > q ? 0L : 0 < p ? p : 1 & (AV(w.n)[0]) ? XNINF : XPINF));
        return z;
    }
    if (q == XPINF || q == XNINF) {
        s = jtxcompare(jt, mag(a.n), a.d);
        QASSERT(0 <= p || 0 > s && q == XPINF || 0 < s && q == XNINF, EVDOMAIN);
        z.n = jtrifvs(jt, jtvci(jt, !s ? 1L : !p && 0 > q || 0 < s && p && 0 < q || 0 > s && 0 > q ? XPINF : 0L));
        return z;
    }
    QASSERT(c || 0 <= p, EWIMAG);
    QASSERT(c || QX1(a.d) && 1 == AN(a.n) && (0 == p || 1 == p), EWIRR);
    if (0 > XDIG(d)) {
        QRE(d = jtnegate(jt, d));
        QRE(t = jtqdiv(jt, z, t));
    }
    if (1 > jtxcompare(jt, d, jtxc(jt, IMAX))) {
        I e;
        QRE(e = jtxint(jt, d));
        while (e) {
            if (1 & e) QRE(z = jtqtymes(jt, z, t));
            QRE(t = jtqtymes(jt, t, t));
            e >>= 1;
        }
    } else {
        X e = d, x2;
        QRE(x2 = jtxc(jt, 2L));
        while (XDIG(e)) {
            if (1 & AV(e)[0]) QRE(z = jtqtymes(jt, z, t));
            QRE(t = jtqtymes(jt, t, t));
            QRE(e = xdiv(e, x2, XMFLR));
        }
    }
    QEPILOG(z);
}

I
jtqcompare(J jt, Q a, Q w) {
    return QCOMP(a, w);
}

static X
jtqbin(J jt, Q a, Q w) {
    ASSERT(QX1(a.d) && QX1(w.d), EWIRR);
    return 0;
}
static D
jtqlogd1(J jt, Q w) {
    ASSERT(0 <= XDIG(w.n), EWIMAG);
    return jtxlogabs(jt, w.n) - jtxlogabs(jt, w.d);
}
static Z
jtqlogz1(J jt, Q w) {
    Z z;
    z.re = jtxlogabs(jt, w.n) - jtxlogabs(jt, w.d);
    z.im = 0 > XDIG(w.n) ? PI : 0.0;
    return z;
}

#define QSQRT(x)              \
    z->n = jtxsqrt(jt, x->n); \
    z->d = jtxsqrt(jt, x->d); \
    {                         \
        I rc;                 \
        if (rc = jt->jerr) {  \
            RESETERR;         \
            return rc;        \
        }                     \
    }

AMON(floorQ, X, Q, *z = xdiv(x->n, x->d, XMFLR);)
AMON(ceilQ, X, Q, *z = xdiv(x->n, x->d, XMCEIL);)
AMON(sgnQ, X, Q, *z = jtxsgn(jt, x->n);)
AMON(absQ, Q, Q, z->n = jtrifvs(jt, mag(x->n)); z->d = x->d;)
AMONPS(sqrtQ, Q, Q, , QSQRT(x), HDR1JERR)
AMONPS(logQD, D, Q, , *z = jtqlogd1(jt, *x);, HDR1JERR)
AMONPS(logQZ, Z, Q, , *z = jtqlogz1(jt, *x);, HDR1JERR)

APFX(maxQQ, Q, Q, Q, QMAX, , return EVOK;)
APFX(minQQ, Q, Q, Q, QMIN, , return EVOK;)
APFX(plusQQ, Q, Q, Q, qplus, , HDR1JERR)
#define qminus(x, y) jtqminus(jt, (x), (y))
APFX(minusQQ, Q, Q, Q, qminus, , HDR1JERR)
#undef qminus
APFX(tymesQQ, Q, Q, Q, qtymes, , HDR1JERR)
#define qdiv(x, y) jtqdiv(jt, (x), (y))
APFX(divQQ, Q, Q, Q, qdiv, , HDR1JERR)
#undef qdiv
#define qgcd(x, y) jtqgcd(jt, (x), (y))
APFX(gcdQQ, Q, Q, Q, qgcd, , HDR1JERR)
#undef ggcd
#define qlcm(x, y) jtqlcm(jt, (x), (y))
APFX(lcmQQ, Q, Q, Q, qlcm, , HDR1JERR)
#undef qlcm
#define qrem(x, y) jtqrem(jt, (x), (y))
APFX(remQQ, Q, Q, Q, qrem, , HDR1JERR)
#undef grem
#define qpow(x, y) jtqpow(jt, (x), (y))
APFX(powQQ, Q, Q, Q, qpow, , HDR1JERR)
#undef qpow
#define qbin(x, y) jtqbin(jt, (x), (y))
APFX(binQQ, X, Q, Q, qbin, , HDR1JERR)
#undef qbin
