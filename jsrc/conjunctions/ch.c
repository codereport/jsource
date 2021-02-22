/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Conjunctions: Hypergeometric Series                                     */

#include "j.h"

static A
jthparm(J jt, A j, A f, A h) {
    A z;
    if (!(VERB & AT(f))) return jtshift1(jt, jtaslash(jt, CSTAR, jtatab(jt, CPLUS, h, j)));
    RZ(z = CALL1(FAV(f)->valencefns[0], j, f));
    ASSERT(1 >= AR(z), EVRANK);
    ASSERT(!AR(z) || AN(j) == AN(z), EVLENGTH);
    return z;
}

static A
jthgv(J jt, B b, I n, A w, A self) {
    A c, d, e, h, *hv, j, y;
    V* sv = FAV(self);
    RZ(j = IX(n));
    h  = sv->fgh[2];
    hv = AAV(h);
    RZ(c = jthparm(jt, j, sv->fgh[0], hv[0]));
    RZ(d = jthparm(jt, j, sv->fgh[1], hv[1]));
    e = jtshift1(jt, divide(w, jtapv(jt, n, 1L, 1L)));
    switch ((VERB & AT(sv->fgh[0]) ? 2 : 0) + (VERB & AT(sv->fgh[1]) ? 1 : 0)) {
        case 0: y = jtascan(jt, CSTAR, divide(tymes(c, e), d)); break;
        case 1: y = divide(jtascan(jt, CSTAR, tymes(c, e)), d); break;
        case 2: y = divide(tymes(c, jtascan(jt, CSTAR, e)), jtascan(jt, CSTAR, d)); break;
        case 3: y = divide(tymes(c, jtascan(jt, CSTAR, e)), d);
    }
    return b ? jtover(jt, jfalse, jtascan(jt, CPLUS, y)) : jtaslash(jt, CPLUS, y);
} /* verb or complex cases */

static A
jthgd(J jt, B b, I n, A w, A p, A q) {
    A c, d, e, z;
    D r, s, t, *u, *v, x, *zv;
    I j, pn, qn;
    RZ(c = jtcvt(jt, FL, p));
    u  = DAV(c);
    pn = AN(c);
    RZ(d = jtcvt(jt, FL, q));
    v  = DAV(d);
    qn = AN(d);
    RZ(e = jtcvt(jt, FL, w));
    x = DAV(e)[0];
    r = s = 1;
    t     = 0;
    z     = 0;
    if (b && 2000 > n) {
        GATV0(z, FL, 1 + n, 1);
        zv    = DAV(z);
        *zv++ = 0;
        *zv++ = 1;
    }
    NAN0;
    for (j = 1; j < n && t != s && !_isnan(s); ++j) {
        DO(pn, r *= u[i]; ++u[i];); /* r*=u[i]++; compiler error on 3B1 */
        DO(qn, r /= v[i]; ++v[i];);
        r *= x / j;
        t = s;
        s += r;
        if (z) *zv++ = s;
        JBREAK0;
    }
    NAN1;
    return !b ? jtscf(jt, s) : z ? jttake(jt, jtsc(jt, 1 + j), z) : jthgd(jt, b, j, w, p, q);
} /* real vector p,q; real scalar w; all terms (1=b) or last term (0=b) */

static A
jthgeom2(J jt, A a, A w, A self) {
    PROLOG(0036);
    A h, *hv, t, z;
    B b;
    I an, *av, j, n;
    V* sv = FAV(self);
    if (AR(w)) return jtrank2ex0(jt, a, w, self, jthgeom2);
    RZ(a = AT(a) & FL + CMPX ? jtvib(jt, a) : jtvi(jt, a));  // kludge just call vib?
    an = AN(a);
    av = AV(a);
    n  = 0;
    DO(an, j = av[i]; ASSERT(0 <= j, EVDOMAIN); if (n < j) n = j;);
    if (!n) return tymes(num(0), a);
    h  = sv->fgh[2];
    hv = AAV(h);
    b  = VERB & (AT(sv->fgh[0]) | AT(sv->fgh[1])) || CMPX & (AT(w) | AT(hv[0]) | AT(hv[1]));
    if (!b)
        z = jthgd(jt, (B)(1 < an), n, w, hv[0], hv[1]);
    else if (2000 > n)
        z = jthgv(jt, (B)(1 < an), n, w, self);
    else {
        j = 10;
        t = mtv;
        z = num(1);
        while (z && !jtequ(jt, z, t)) {
            t = z;
            z = jthgv(jt, 0, j, w, self);
            j += j;
        }
        RZ(z);
        if (1 < an) z = jthgv(jt, 1, j, w, self);
    }
    if (1 < an) z = jtfrom(jt, minimum(a, jtsc(jt, SETIC(z, an) - 1)), z);
    EPILOG(z);
}

static A
jthgeom1(J jt, A w, A self) {
    return jthgeom2(jt, jtsc(jt, IMAX), w, self);
}

static A
jtcancel(J jt, A a, A w) {
    A c, d, f, x, y;
    f = jteval(jt, "#/.~");  // could call keytally
    a = jtravel(jt, a);
    x = jtnub(jt, a);
    df1(c, a, f);
    w = jtravel(jt, w);
    y = jtnub(jt, w);
    df1(d, w, f);
    a = jtrepeat(jt, maximum(jfalse, minus(c, jtfrom(jt, jtindexof(jt, y, x), jtover(jt, d, num(0))))), x);
    w = jtrepeat(jt, maximum(jfalse, minus(d, jtfrom(jt, jtindexof(jt, x, y), jtover(jt, c, num(0))))), y);
    return link(a, w);
}

A
jthgeom(J jt, A a, A w) {
    A c, d, h = 0;
    B p, q;
    I at, wt;
    at = AT(a);
    p  = 1 && at & NOUN;
    c = d = mtv;
    wt    = AT(w);
    q     = 1 && wt & NOUN;
    if (p) {
        c = a;
        ASSERT(!AN(a) || at & NUMERIC, EVDOMAIN);
        ASSERT(1 >= AR(a), EVRANK);
    }
    if (q) {
        d = w;
        ASSERT(!AN(w) || wt & NUMERIC, EVDOMAIN);
        ASSERT(1 >= AR(w), EVRANK);
    }
    RZ(h = jtcancel(jt, c, d));
    return jtfdef(jt, 0, CHGEOM, VERB, jthgeom1, jthgeom2, a, w, h, 0L, 0L, 0L, 0L);
} /* a H. w */
