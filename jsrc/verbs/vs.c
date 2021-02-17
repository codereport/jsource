/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: $. Sparse Arrays                                                 */

#include "j.h"

B
jtscheck(J jt, A w) {
    A a, e, x, y;
    I k1, k2, r, *s, t;
    P* p;
    D rkblk[16];
    r = AR(w);
    s = AS(w);
    t = AT(w);
    if (t & DENSE) return 1;
    ASSERTSYS(r, "scheck rank");
    DO(r, ASSERTSYS(0 <= s[i], "scheck shape"););
    p = PAV(w);
    a = SPA(p, a);
    e = SPA(p, e);
    y = SPA(p, i);
    x = SPA(p, x);
    ASSERTSYS(a, "scheck a missing");
    ASSERTSYS(e, "scheck e missing");
    ASSERTSYS(y, "scheck i missing");
    ASSERTSYS(x, "scheck x missing");
    ASSERTSYS(1 == AR(a), "scheck a rank");
    ASSERTSYS(all1(jteps(jt, a, IX(r))), "scheck a index");
    ASSERTSYS(jtequ(jt, a, jtnub(jt, a)), "scheck a unique");
    ASSERTSYS(!AR(e), "scheck e rank");
    ASSERTSYS(TYPESEQ(AT(e), DTYPE(t)), "scheck e type");
    ASSERTSYS(TYPESEQ(AT(e), AT(x)), "scheck e/x type");
    ASSERTSYS(2 == AR(y), "scheck i rank");
    ASSERTSYS(INT & AT(y), "scheck i type");
    ASSERTSYS(SETIC(y, k1) == SETIC(x, k2), "scheck i/x tally");
    ASSERTSYS(*(1 + AS(y)) == SETIC(a, k1), "scheck i/a length");
    ASSERTSYS(jtequ(jt, y, jtnub(jt, y)), "scheck i unique");
    ASSERTSYS(all1(le(num(0), y)), "scheck i negative");
    ASSERTSYS(all1(ATOMIC2(jt, y, jtfrom(jt, a, shape(jt, w)), rkblk, 1L, 1L, CLT)), "scheck i index");
    ASSERTSYS(jtequ(jt, jtgrade1(jt, y), IX(*AS(y))), "scheck i sorted");
    ASSERTSYS(AR(x) == 1 + r - AN(a), "scheck x rank");
    ASSERTSYS(jtequ(jt, jtbehead(jt, shape(jt, x)), jtfrom(jt, jtless(jt, IX(r), a), shape(jt, w))), "scheck x shape");
    return 1;
} /* assertions on sparse array w */

static A
jtselm(J jt, I t) {
    return t & NUMERIC ? jtcvt(jt, t, num(0)) : t & BOX ? ds(CACE) : chrspace;
}

A
jtpaxis(J jt, I r, A a) {
    A y, z;
    B* b;
    I j, *u, *v;
    if (!(INT & AT(a))) RZ(a = jtcvt(jt, INT, a));
    u = AV(a);
    GATV0(y, B01, r, 1);
    b = BAV(y);
    memset(b, C0, r);
    DO(AN(a), j = u[i]; b[0 > j ? j + r : j] = 1;);
    GATV0(z, INT, r, 1);
    v = AV(z);
    DO(r, if (b[i])* v++ = i;);
    DO(r, if (!b[i])* v++ = i;);
    return z;
} /* permuted axes per sparse axes specification a */

static A
jtvaxis(J jt, I r, A a) {
    A y;
    B* b;
    I j, n, *v;
    RZ(a = jtcvt(jt, INT, a));
    n = AN(a);
    v = AV(a);
    ASSERT(1 >= AR(a), EVRANK);
    GATV0(y, B01, r, 1);
    b = BAV(y);
    memset(b, C0, r);
    DO(n, j = v[i]; if (0 > j) j += r; ASSERT(0 <= j && j < r && !b[j], EVINDEX); b[j] = 1;);
    return jtcaro(jt, jtifb(jt, r, b));  // avoid readonly
} /* standardize axes to be non-negative, sorted */

A
jtdaxis(J jt, I r, A a) {
    return jtless(jt, IX(r), a);
}
/* dense axes relative to sparse axes a */

// e is sparse element
static A
jtsparse1a(J jt, A s, A a, A e, A y, A x) {
    A z;
    B* b;
    I an, *av, et, r, *sv, t, *v;
    P* p;
    RZ(s && a && e);
    RZ(s = jtvi(jt, s));
    r  = AN(s);
    sv = AV(s);
    ASSERT(1 >= AR(s), EVRANK);
    ASSERT(r != 0, EVLENGTH);
    ASSERT(r <= RMAX, EVLIMIT);
    DO(r, ASSERT(0 <= sv[i], EVDOMAIN););
    RZ(a = jtvaxis(jt, r, a == mark ? IX(r) : a));
    an = AN(a);
    av = AV(a);
    if (e == mark) RZ(e = jtscf(jt, 0.0));
    ASSERT(!AR(e), EVRANK);  // e must be an atom
    et = AT(e);
    ASSERT(!(et & LIT + BOX), EVNONCE);  // e must be numeric
    ASSERT(STYPE(et) != 0, EVDOMAIN);    // e must be dense
    RZ(b = bfi(r, a, 0));
    if (y == mark) {
        GAT0(y, INT, 0L, 2L);
        v    = AS(y);
        v[0] = 0;
        v[1] = an;
        GA(x, et, 0L, 1 + r - an, 0L);
        v    = AS(x);
        v[0] = 0;
        DO(r, if (b[i])* ++v = sv[i];);
    } else {
        A q, x1, y1;
        C *xu, *xv;
        I i, j, k, m, n, *qv, *u, *yu, *yv;
        ASSERT(2 == AR(y), EVRANK);
        ASSERT(an == *(1 + AS(y)), EVLENGTH);
        if (!(INT & AT(y))) RZ(y = jtcvt(jt, INT, y));
        GATV0(q, INT, an, 1);
        qv = AV(q);
        DO(an, qv[i] = sv[av[i]];);
        u = AV(y);
        DO(*AS(y), DO(an, j = *u++; ASSERT(0 <= j && j < qv[i], EVINDEX);););
        ASSERT(AR(x) == 1 + r - an, EVRANK);
        v = AS(x);
        DO(
          r, if (b[i]) {
              j = *++v;
              ASSERT(j == sv[i], EVLENGTH);
          });
        ASSERT(*AS(x) == *AS(y), EVLENGTH);
        ASSERT(HOMO(et, AT(x)), EVDOMAIN);
        t = maxtype(et, AT(x));
        if (TYPESNE(t, et)) RZ(e = jtcvt(jt, t, e));
        if (TYPESNE(t, AT(x))) RZ(x = jtcvt(jt, t, x));
        n = *AS(y) - 1;
        u = AV(y);
        v = an + u;
        for (i = 0; i < n; ++i) {
            j = 0;
            DO(
              an,
              if (u[i] < v[i]) {
                  j = -1;
                  break;
              } else if (u[i] > v[i]) {
                  j = 1;
                  break;
              });
            if (0 <= j) break;
            u += an;
            v += an;
        }
        if (n && 0 <= j) {
            m = jtaii(jt, x);
            k = m << bplg(t);
            RZ(q = jtgrade1(jt, y));
            qv = AV(q);
            GATV(y1, INT, AN(y), AR(y), AS(y));
            yv = AV(y1);
            yu = AV(y);
            ICPY(yv, yu + an * *qv, an);
            GA(x1, t, AN(x), AR(x), AS(x));
            xv = CAV(x1);
            xu = CAV(x);
            memcpy(xv, xu + k * *qv, k);
            for (i = 0; i < n; ++i) {
                ++qv;
                v = yu + an * *qv;
                DO(
                  an, if (yv[i] < v[i]) {
                      yv += an;
                      ICPY(yv, v, an);
                      xv += k;
                      memcpy(xv, xu + k * *qv, k);
                      break;
                  });
            }
            yv += an;
            AN(y1)  = yv - AV(y1);
            *AS(y1) = AN(y1) / an;
            y       = y1;
            xv += k;
            *AS(x1) = (xv - CAV(x1)) / k;
            AN(x1)  = m * *AS(x1);
            x       = x1;
        }
    }
    t = STYPE(AT(x));
    ASSERT(t != 0, EVDOMAIN);
    GASPARSE(z, t, 1, r, sv);
    p = PAV(z);
    SPB(p, a, a);
    SPB(p, e, e);
    SPB(p, i, y);
    SPB(p, x, x);
    return z;
}

// convert w to sparse, with sparse element e (which must be a dense atom)
A
jtsparseit(J jt, A w, A a, A e) {
    PROLOG(0091);
    A ax, c, x, y, z;
    B b, *cv;
    I cm, cn, m, n, r, *s, t, *u, *v, wn;
    P* p;
    RZ(w && a && e);
    r  = AR(w);
    t  = AT(w);
    wn = AN(w);
    n  = AN(a);
    ASSERT(!(t & LIT + BOX), EVNONCE);  // must not be LIT or BOX
    ASSERT(STYPE(t) != 0, EVDOMAIN);    // w must be dense
    if (!r) {
        ASSERT(!AN(a), EVINDEX);
        return jtca(jt, w);
    }
    RZ(z = sparse1a(shape(jt, w), a, e, mark, mark));
    p = PAV(z);
    RZ(ax = jtpaxis(jt, r, a));
    GATV0(y, INT, r, 1);
    s = AV(y);
    u = AV(ax);
    v = AS(w);
    DO(r, s[i] = v[u[i]];);
    RE(m = jtprod(jt, n, s));
    b = jtequ(jt, a, IX(r));
    RZ(x = virtual(b ? w : jtcant2(jt, ax, w), 0, 1 + r - n));
    AN(x) = AN(w);
    v     = AS(x);
    *v    = m;
    if (r > n) ICPY(1 + v, n + s, r - n);
    b = b && SB01 & AT(z) && jtequ(jt, e, num(0));
    c = w;
    if (!b) RZ(c = jtnot(jt, irs2(jtreshape(jt, vec(INT, r - n, n + s), SPA(p, e)), x, VFLAGNONE, RMAX, -1L, jtmatch)));
    cn = AN(c);
    cv = BAV(c);
    cm = bsum(cn, cv);
    /* RZ(y=jtabase2(jt,vec(INT,n,s),jtrepeat(jt,c,IX(cn)))); */
    GATV0(y, INT, cm * n, 2);
    u    = AS(y);
    *u++ = cm;
    *u   = n;
    if (cm) {
        I d, e, k, q, *sn, *yv;
        k = cn - 1;
        cv += cn;
        yv = AN(y) + AV(y);
        sn = s + n;
        d  = *(sn - 1);
        e  = *(sn - 2);
        switch (n) {
            case 1:
                cv = BAV(c);
                yv = AV(y);
                DO(cn, if (*cv++)* yv++ = i;);
                break;
            case 2:
                DO(
                  cn, if (*--cv) {
                      q     = k - i;
                      *--yv = q % d;
                      *--yv = q / d;
                  });
                break;
            case 3:
                DO(
                  cn, if (*--cv) {
                      q     = k - i;
                      *--yv = q % d;
                      q /= d;
                      *--yv = q % e;
                      *--yv = q / e;
                  });
                break;
            default:
                DO(
                  cn, if (*--cv) {
                      q = k - i;
                      u = sn;
                      DQ(n, d = *--u; *--yv = q % d; q /= d;);
                  });
        }
    }
    SPB(p, i, y);
    SPB(p, x, b ? jtreshape(jt, jtsc(jt, cm), num(1)) : jtrepeat(jt, c, x));
    EPILOG(z);
}

A
jtdenseit(J jt, A w) {
    A a, e, q, s1, x, y, z;
    B b;
    C *xv, *zv;
    I an, ck, k, n, r, t, *s, xn, *yv;
    P* wp;
    r = AR(w);
    t = AT(w);
    if (!r || t & DENSE) return jtca(jt, w);
    t  = DTYPE(t);
    wp = PAV(w);
    a  = SPA(wp, a);
    e  = SPA(wp, e);
    x  = SPA(wp, x);
    y  = SPA(wp, i);
    xn = AN(x);
    an = AN(a);
    b  = jtequ(jt, a, IX(an));
    if (!an || !xn) return jtreshape(jt, shape(jt, w), xn ? x : e);
    if (b)
        s = AS(w);
    else {
        RZ(q = jtover(jt, a, jtless(jt, IX(r), a)));
        RZ(s1 = jtfrom(jt, q, shape(jt, w)));
        s = AV(s1);
    }
    RE(n = jtprod(jt, r, s));
    GA(z, t, n, r, s);
    zv = CAV(z);
    xv = CAV(x);
    if (1 < an) RZ(y = jtbase2(jt, vec(INT, an, s), y));
    yv = AV(y);
    k  = bpnoun(t);
    ck = k * jtaii(jt, x);
    mvc(k * n, zv, k, AV(e));
    DQ(SETIC(y, an), memcpy(zv + ck * *yv, xv, ck); ++yv; xv += ck;);
    return b ? z : jtcant2(jt, jtpinv(jt, q), z);
} /* $.^:_1 */

A
jtreaxis(J jt, A a, A w) {
    A a1, e, p, q, x, y, z;
    B* b;
    I c, d, j, k, m, r, *u, *v, *ws, wt;
    P *wp, *zp;
    wt = AT(w);
    if (wt & DENSE) return sparseit(w, a, jtselm(jt, wt));
    r  = AR(w);
    ws = AS(w);
    wp = PAV(w);
    GASPARSE(z, wt, 1L, r, ws);
    zp = PAV(z);
    SPBV(zp, a, a1, jtvaxis(jt, r, a));
    SPBV(zp, e, e, jtca(jt, SPA(wp, e)));
    a = SPA(wp, a);
    x = SPA(wp, x);
    y = SPA(wp, i);
    m = *AS(y);
    if (all1(jteps(jt, a, a1))) {
        I* s; /* old is subset of new */
        RZ(p = jteps(jt, jtdaxis(jt, r, a), a1));
        b = BAV(p);
        GATV0(q, INT, 1 + r, 1);
        u = AV(q);
        j = 1;
        GATV0(q, INT, 1 + r, 1);
        v = AV(q);
        k = 0;
        s = AS(x);
        c = 1;
        DO(
          AN(p), d = s[1 + i]; if (b[i]) {
              c *= d;
              v[k++] = d;
          } else u[j++] = d;);
        *u = c * m;
        RZ(x = jtreshape(jt, vec(INT, j, u), jtcant2(jt, jtincrem(jt, jtdgrade1(jt, p)), x)));
        RZ(q = jtnot(jt, irs2(x, jtreshape(jt, vec(INT, AR(x) - 1, 1 + AS(x)), e), 0L, -1L, RMAX, jtmatch)));
        SPBV(zp, x, x, jtrepeat(jt, q, x));
        RZ(y = jtstitch(
             jt, jtrepeat(jt, jtsc(jt, c), y), jtreshape(jt, jtv2(jt, c * m, k), jtabase2(jt, vec(INT, k, v), IX(c)))));
        RZ(p = jtgrade1(jt, jtover(jt, a, jtless(jt, a1, a))));
        if (jtequ(jt, p, IX(AN(p))))
            SPB(zp, i, jtrepeat(jt, q, y))
        else {
            y = jtfromr(jt, p, jtrepeat(jt, q, y));
            q = jtgrade1(jt, y);
            SPB(zp, i, jtfrom(jt, q, y));
            SPB(zp, x, jtfrom(jt, q, x));
        }
        return z;
    }
    if (all1(jteps(jt, a1, a))) {
        A x1, y1;
        B* pv;
        C *s, *t;
        I g, h, *iv, n; /* new is subset of old */
        c = AN(a);
        d = AN(a1);
        RZ(p = jteps(jt, a, a1));
        RZ(y = jtfromr(jt, jtdgrade1(jt, p), y));
        RZ(q = jtgrade1(jt, y));
        RZ(y = jtfrom(jt, q, y));
        RZ(x = jtfrom(jt, q, x));
        GATV0(q, B01, m, 1);
        b = BAV(q);
        n = 0;
        if (m) {
            b[m - 1] = 1;
            n        = 1;
            u        = AV(y);
            DO(m - 1, if (b[i] = 1 && ICMP(u, u + c, d))++ n; u += c;);
        }
        GATV0(q, INT, 1 + r, 1);
        u  = AV(q);
        j  = 0;
        v  = AV(a);
        pv = BAV(p);
        DO(AN(p), if (!pv[i]) u[j++] = ws[v[i]];);
        RE(jtprod(jt, j, u));
        u[j] = k = 1;
        DQ(c - d, --j; u[j] = k *= u[j];);
        RZ(q = jtpdt(jt, jttake(jt, jtv2(jt, m, d - c), y), vec(INT, c - d, 1 + u)));
        iv = AV(q);
        RZ(p = jtover(jt, jtless(jt, a, a1), jtdaxis(jt, r, a)));
        v  = AV(p);
        *u = n;
        j  = 1;
        DQ(AN(p), u[j++] = ws[*v++];);
        RE(h = jtprod(jt, 1 + r - d, u));
        GA(x1, AT(x), h, 1 + r - d, u);
        t = CAV(x1);
        s = CAV(x);
        GATV0(y1, INT, n * d, 2);
        *AS(y1)       = n;
        *(1 + AS(y1)) = d;
        v             = AV(y1);
        u             = AV(y);
        k             = bpnoun(AT(x));
        g             = k * jtaii(jt, x);
        h             = k * jtaii(jt, x1);
        mvc(k * AN(x1), t, k, AV(e));
        DO(
          m, memcpy(t + g * iv[i], s, g); s += g; if (b[i]) {
              ICPY(v, u + i * c, d);
              v += d;
              t += h;
          });
        SPB(zp, i, y1);
        SPB(zp, x, jtcant2(jt, jtincrem(jt, jtindexof(jt, p, jtdaxis(jt, r, a1))), x1));
        return z;
    }
    return jtreaxis(jt, a1, jtreaxis(jt, jtover(jt, a, jtless(jt, a1, a)), w));
} /* (2;a)$.w */

static A
jtaxbytes1(J jt, I t, I an, I m, I xr, I* xs) {
    I k, z;
    k = bpnoun(t);
    z = SZI * NORMAH + SZI * (an + xr) + sizeof(P);
    z += SZI * NORMAH + k;
    z += SZI * NORMAH + SZI * (1 + xr) + k * m * jtprod(jt, xr, xs);
    z += SZI * NORMAH + SZI * 2 + SZI * m * an;
    return jtsc(jt, z);
}

static A
jtaxbytes(J jt, A a, A w) {
    A a1, e, p, q, x;
    B* b;
    I c, d, j, m, n = 0, r, *u, *v, *ws, wt;
    P* wp;
    r  = AR(w);
    ws = AS(w);
    wt = AT(w);
    GATV0(q, INT, r, 1);
    u = AV(q);
    j = 0;
    RZ(a1 = jtvaxis(jt, r, a));
    d = AN(a1);
    if (wt & SPARSE) {
        wp = PAV(w);
        a  = SPA(wp, a);
        e  = SPA(wp, e);
        x  = SPA(wp, x);
        c  = 1;
    } else {
        a = mtv;
        RZ(e = jtselm(jt, wt));
        x = w;
        c = 0;
    }
    if (all1(jteps(jt, a, a1))) { /* old is subset of new */
        RZ(p = jteps(jt, jtdaxis(jt, r, a), a1));
        b = BAV(p);
        v = c + AS(x);
        DO(AN(p), if (!b[i]) u[j++] = v[i];);
        RZ(q = irs2(
             jtcant2(jt, plus(jtsc(jt, c), jtdgrade1(jt, p)), x), jtreshape(jt, vec(INT, j, u), e), 0L, j, j, jtmatch));
        b = BAV(q);
        n = AN(q);
        DQ(n, if (*b++)-- n;);
        return axbytes1(AT(e), d, n, j, u);
    }
    if (all1(jteps(jt, a1, a))) {
        A y = SPA(wp, i); /* new is subset of old */
        RZ(y = jtfromr(jt, jtindexof(jt, a, a1), y));
        RZ(y = jtgrade2(jt, y, y));
        if (m = *AS(y)) {
            n = 1;
            u = AV(y);
            DQ(m - 1, if (ICMP(u, u + d, d))++ n; u += d;);
        }
        RZ(p = jtover(jt, jtless(jt, a, a1), jtdaxis(jt, r, a)));
        v = AV(p);
        DQ(AN(p), u[j++] = ws[*v++];);
        return axbytes1(AT(e), d, n, j, u);
    }
    return jtaxbytes(jt, a1, jtreaxis(jt, jtover(jt, a, jtless(jt, a1, a)), w));
} /* bytes required for (2;a)$.w */

static A
jtaxtally(J jt, A a, A w) {
    A a1, e, p, q, x;
    B* b;
    I c, d, j, m, n = 0, r, *u, *v, *ws, wt;
    P* wp;
    r  = AR(w);
    ws = AS(w);
    wt = AT(w);
    GATV0(q, INT, r, 1);
    u = AV(q);
    j = 0;
    RZ(a1 = jtvaxis(jt, r, a));
    d = AN(a1);
    if (wt & SPARSE) {
        wp = PAV(w);
        a  = SPA(wp, a);
        e  = SPA(wp, e);
        x  = SPA(wp, x);
        c  = 1;
    } else {
        a = mtv;
        RZ(e = jtselm(jt, wt));
        x = w;
        c = 0;
    }
    if (all1(jteps(jt, a, a1))) { /* old is subset of new */
        RZ(p = jteps(jt, jtdaxis(jt, r, a), a1));
        b = BAV(p);
        v = c + AS(x);
        DO(AN(p), if (!b[i]) u[j++] = v[i];);
        RZ(q = irs2(
             jtcant2(jt, plus(jtsc(jt, c), jtdgrade1(jt, p)), x), jtreshape(jt, vec(INT, j, u), e), 0L, j, j, jtmatch));
        b = BAV(q);
        n = AN(q);
        DQ(n, if (*b++)-- n;);
        return jtsc(jt, n);
    }
    if (all1(jteps(jt, a1, a))) {
        A y = SPA(wp, i); /* new is subset of old */
        RZ(y = jtfromr(jt, jtindexof(jt, a, a1), y));
        RZ(y = jtgrade2(jt, y, y));
        if (m = *AS(y)) {
            n = 1;
            u = AV(y);
            DQ(m - 1, if (ICMP(u, u + d, d))++ n; u += d;);
        }
        return jtsc(jt, n);
    }
    return jtaxtally(jt, a1, jtreaxis(jt, jtover(jt, a, jtless(jt, a1, a)), w));
} /* #4$.(2;a)$.w */

A
jtrezero(J jt, A a, A w) {
    A x, z;
    I at, t, wt, zt;
    P *wp, *zp;
    at = AT(a);
    wp = PAV(w);
    x  = SPA(wp, x);
    wt = AT(x);
    ASSERT(!AR(a), EVRANK);
    ASSERT(HOMO(at, wt), EVDOMAIN);
    RE(t = maxtype(at, wt));
    zt = STYPE(t);
    ASSERT(zt != 0, EVDOMAIN);
    GASPARSE(z, zt, 1, AR(w), AS(w));
    zp = PAV(z);
    SPB(zp, e, TYPESEQ(t, at) ? jtca(jt, a) : jtcvt(jt, t, a));
    SPB(zp, a, jtca(jt, SPA(wp, a)));
    SPB(zp, i, jtca(jt, SPA(wp, i)));
    SPB(zp, x, TYPESEQ(t, wt) ? jtca(jt, x) : jtcvt(jt, t, x));
    return z;
} /* respecify the sparse element */

A
jtunzero(J jt, A w) {
    A e, q, x, z;
    I r;
    P *wp, *zp;
    wp = PAV(w);
    e  = SPA(wp, e);
    x  = SPA(wp, x);
    r  = AR(x) - 1;
    GASPARSE(z, AT(w), 1, AR(w), AS(w));
    zp = PAV(z);
    RZ(q = jtnot(jt, irs2(x, jtreshape(jt, vec(INT, r, 1 + AS(x)), e), 0L, r, r, jtmatch)));
    SPB(zp, x, jtrepeat(jt, q, x));
    SPB(zp, i, jtrepeat(jt, q, SPA(wp, i)));
    SPB(zp, a, jtca(jt, SPA(wp, a)));
    SPB(zp, e, jtca(jt, e));
    return z;
} /* remove completely sparse cells */

static A
jtsparsep1(J jt, A w) {
    A* wv;
    I n = 0;
    ASSERT(1 >= AR(w), EVRANK);
    if (BOX & AT(w)) {
        n  = AN(w);
        wv = AAV(w);
        ASSERT(1 <= n && n <= 3 || 5 == n, EVLENGTH);
    }
    return sparse1a(
      0 < n ? wv[0] : w, 1 < n ? wv[1] : mark, 2 < n ? wv[2] : mark, 3 < n ? wv[3] : mark, 4 < n ? wv[4] : mark);
}

static A
jtsparsen1(J jt, A w) {
    A *u, z;
    P* p;
    ASSERT(SPARSE & AT(w), EVDOMAIN);
    GAT0(z, BOX, 3, 1);
    u    = AAV(z);
    p    = PAV(w);
    u[0] = shape(jt, w);
    u[1] = jtca(jt, SPA(p, a));
    u[2] = jtca(jt, SPA(p, e));
    RE(0);
    return z;
}

A
jtsparse1(J jt, A w) {
    if (!AR(w) || SPARSE & AT(w)) return jtca(jt, w);
    return sparseit(w, IX(AR(w)), jtselm(jt, AT(w)));
} /* $. y */

A
jtsparse2(J jt, A a, A w) {
    A *av, q = 0;
    B b;
    I j, k, t, *v;
    P* p;
    if (BOX & AT(a)) {
        ASSERT(1 == AR(a), EVRANK);
        ASSERT(2 == AN(a), EVLENGTH);
        av = AAV(a);
        a  = av[0];
        q  = av[1];
    }
    RZ(a = jtcvt(jt, INT, a));
    ASSERT(1 >= AR(a), EVRANK);
    v = AV(a);
    k = *v;
    ASSERT(2 == k || !AR(a), EVRANK);
    ASSERT(2 >= AN(a), EVLENGTH);
    p = PAV(w);
    t = AT(w);
    b = 1 && t & SPARSE;
    ASSERT(b || 0 <= k && k <= 2, EVDOMAIN);
    switch (k) {
        case 0: ASSERT(!q, EVDOMAIN); return t & SPARSE ? jtdenseit(jt, w) : jtsparse1(jt, w);
        case 1:
            ASSERT(!q, EVDOMAIN);
            q = jtsparsep1(jt, w);
            PRISTCLRF(w);
            return q;
        case -1: ASSERT(!q, EVDOMAIN); return jtsparsen1(jt, w);
        case 2:
            if (AR(a)) {
                j = v[1];
                ASSERT(q && (1 == j || 2 == j), EVDOMAIN);
                return 1 == j ? jtaxbytes(jt, q, w) : jtaxtally(jt, q, w);
            }
            if (q)
                return jtreaxis(jt, q, w);
            else if (b)
                return jtrat(jt, SPA(p, a));
            else {
                ASSERT(STYPE(t) != 0, EVDOMAIN);
                return IX(AR(w));
            }
        case 3:
            return q ? jtrezero(jt, q, w)
                     : jtrat(jt, SPA(p, e));  // ? there jtrat(jt,)s don't protect anything?  SPA is as permanent as w
        case 4: ASSERT(!q, EVDOMAIN); return jtrat(jt, SPA(p, i));
        case 5: ASSERT(!q, EVDOMAIN); return jtrat(jt, SPA(p, x));
        case 7: ASSERT(!q, EVDOMAIN); return jtsc(jt, SETIC(SPA(p, i), j));
        case 8: ASSERT(!q, EVDOMAIN); return jtunzero(jt, w);
        default: ASSERT(0, EVDOMAIN);
    }
} /* x $. y */
