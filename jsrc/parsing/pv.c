/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Parsing: Tacit Verb Translator (13 : )                                  */

#include "j.h"
#include "p.h"

#define WTA 2 /* sizeof(TA)/sizeof(A) */
#define NTTAB 20
#define TC 5485900

#define CHK1 (!(stack[b].t))
#define CHK2 (!(stack[b].t || stack[e].t))
#define CHK3 (!(stack[b].t || stack[1 + b].t || stack[e].t))
#define CP ds(CCAP)
#define DCASE(x, y) (6 * (x) + (y))
#define FGL(v) jtfolk(jt, v->fgh[0], v->fgh[1], ds(CLEFT))
#define FGR(v) jtfolk(jt, v->fgh[0], v->fgh[1], ds(CRIGHT))
#define LF ds(CLEFT)
#define RT ds(CRIGHT)
#define RZZ(exp)               \
    {                          \
        if (!(exp)) return zz; \
    }

static const TA zz = {0, 0};

// Returns AM(z) 1 if monad, 0 if dyad
static A
jtvtokens(J jt, A w) {
    A t, *y, z;
    I n, *s;
    TA *x;
    RZ(t = jttokens(jt, jtvs(jt, w), 0));
    n        = AN(t);
    y        = AAV(t);
    I tmonad = 1;
    GATV0(z, BOX, WTA * (5 + n), 2);
    s    = AS(z);
    *s++ = 5 + n;
    *s   = WTA;
    x    = (TA *)AV(z);
    x->a = mark;
    x->t = 0;
    ++x;
    DQ(n, x->a = t = *y++; x->t = 0; ++x; if (AT(t) & NAME && NAV(t)->flag & NMDOT && NAV(t)->s[0] == 'x') tmonad = 0;);
    x->a = mark;
    x->t = 0;
    ++x;
    x->a = mark;
    x->t = 0;
    ++x;
    x->a = mark;
    x->t = 0;
    ++x;
    x->a = mark;
    x->t = 0;
    ++x;
    AM(z) = tmonad;
    return z;
} /* build string sentence into queue suitable for parsing */

static A
jtcfn(J jt, A w) {
    I j;
    return !AR(w) && INT & AT(w) && (j = AV(w)[0], -9 <= j && j <= 9) ? FCONS(w) : jtqq(jt, w, ainf);
}
/* constant function with value w */

static A
jttine(J jt, A w, I tmonad) {
    V *v;
    return w && tmonad && (v = FAV(w), CP == v->fgh[0] && RT == v->fgh[2]) ? v->fgh[1] : w;
}
/* if monad and w = [: g ], then g; else just w itself */

static I
tvi(A w) {
    A x;
    I i, z = -1;
    V *v;
    if (w && VERB & AT(w)) {
        v = FAV(w);
        if (CQQ == v->id && num(-1) == v->fgh[1]) {
            x = v->fgh[0];
            if (!AR(x) && INT & AT(x)) {
                i = AV(x)[0] - TC;
                z = 0 <= i && i < NTTAB ? i : -1;
            }
        }
    }
    return z;
}

static const C ctab[] = {CEQ, CMIN, CMAX, CPLUS, CPLUSDOT, CPLUSCO, CSTAR, CSTARDOT, CSTARCO, CMATCH, CNE, 0};

static A
jtswapc(J jt, A w) {
    C c;
    if (!jtprimitive(jt, w)) return jtswap(jt, w);
    c = ID(w);
    return strchr(ctab, c) ? w
           : c == CLT      ? ds(CGT)
           : c == CGT      ? ds(CLT)
           : c == CLE      ? ds(CGE)
           : c == CGE      ? ds(CLE)
                           : jtswap(jt, w);
} /* w~ or equivalent */

TA
jtvmonad(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0) {
    A fs;
    TA y, z = {jtrue, 0};
    V *v;
    y  = stack[e];
    fs = stack[b].a;
    if (!y.t)
        df1(z.a, y.a, fs);
    else {
        v = FAV(y.t);
        if (!(CFORK == v->id && 0 <= tvi(v->fgh[2])))
            z.t = jtfolk(jt, CP, fs, tine(y.t));
        else if (NOUN & AT(v->fgh[0]))
            z.t = jtfolk(jt, CP, jtfolk(jt, CP, fs, jtfolk(jt, v->fgh[0], v->fgh[1], RT)), tine(v->fgh[2]));
        else
            z.t = jtfolk(jt, tine(v->fgh[0]), jtfolk(jt, CP, fs, v->fgh[1]), tine(v->fgh[2]));
    }
    return z;
}

static I
jtdcase(J jt, I xi, V *v) {
    return !v ? 0 : 0 > xi ? 1 : CFORK != v->id ? 2 : NOUN & AT(v->fgh[0]) ? 3 : CP == v->fgh[0] ? 4 : 5;
}
/* 0   x        */
/* 1      f     */
/* 2         t  */
/* 3   x  f  t  */
/* 4   [: f  t  */
/* 5   s  f  t  */

TA
jtvdyad(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0) {
    A fs, sf, xt, yt;
    B xl, xr, yl, yr;
    I xi = -1, yi = -1;
    TA x, y, z = {jtrue, 0};
    V *u = 0, *v = 0;
    fs = stack[e - 1].a;
    x  = stack[b];
    y  = stack[e];
    sf = jtswapc(jt, fs);
    if (xt = tine(x.t)) {
        xi = tvi(x.t);
        u  = FAV(xt);
        if (0 > xi && CFORK == u->id) {
            xi = tvi(u->fgh[0]);
            if (0 > xi) xi = tvi(u->fgh[2]);
        }
    }
    if (yt = tine(y.t)) {
        yi = tvi(y.t);
        v  = FAV(yt);
        if (0 > yi && CFORK == v->id) {
            yi = tvi(v->fgh[0]);
            if (0 > yi) yi = tvi(v->fgh[2]);
        }
    }
    if (fs == ds(CLEFT)) {
        if (xt)
            z.t = xt;
        else
            z.a = x.a;
        return z;
    }
    if (0 > xi && 0 > yi) switch ((xt ? 2 : 0) + (yt ? 1 : 0)) {
            case 0: df2(z.a, x.a, y.a, fs); break;
            case 1: z.t = jtfolk(jt, x.a, fs, yt); break;
            case 2: z.t = jtfolk(jt, y.a, sf, xt); break;
            case 3:
                xl = xt == LF;
                xr = xt == RT;
                yl = yt == LF;
                yr = yt == RT;
                if (xl && yr)
                    z.t = fs;
                else if (xr && yl)
                    z.t = sf;
                else if (xr && yr && tmonad)
                    z.t = jtswap(jt, fs);
                else
                    z.t = CFORK == u->id && jtprimitive(jt, yt) ? jtfolk(jt, yt, sf, xt) : jtfolk(jt, xt, fs, yt);
        }
    else {
        B b, c;
        I i, j, xj, yj;
        i = jtdcase(jt, xi, u);
        if (u && CFORK == u->id) {
            xi = tvi(u->fgh[0]);
            xj = tvi(u->fgh[2]);
        } else {
            xi = -1;
            xj = tvi(xt);
        }
        j = jtdcase(jt, yi, v);
        if (v && CFORK == v->id) {
            yi = tvi(v->fgh[0]);
            yj = tvi(v->fgh[2]);
        } else {
            yi = -1;
            yj = tvi(yt);
        }
        z.t = 0;
        b   = xj == yj;
        c   = xj == yi;
        switch (DCASE(i, j)) {
            case DCASE(0, 2): z.t = jtfolk(jt, x.a, fs, yt); break;
            case DCASE(2, 0): z.t = jtfolk(jt, y.a, sf, xt); break;
            case DCASE(0, 3): z.t = jtfolk(jt, CP, jtfolk(jt, x.a, fs, FGR(v)), v->fgh[2]); break;
            case DCASE(0, 4): z.t = jtfolk(jt, CP, jtfolk(jt, x.a, fs, v->fgh[1]), v->fgh[2]); break;
            case DCASE(1, 2): z.t = jtfolk(jt, xt, fs, yt); break;
            case DCASE(1, 3):
            case DCASE(1, 4): z.t = jtfolk(jt, xt, jtfolk(jt, LF, fs, FGR(v)), v->fgh[2]); break;
            case DCASE(2, 1): z.t = jtfolk(jt, xt, fs, yt); break;
            case DCASE(3, 1): z.t = jtfolk(jt, xt, fs, yt); break;
            case DCASE(4, 1): z.t = jtfolk(jt, xt, fs, yt); break;
            case DCASE(2, 2): z.t = jtfolk(jt, xt, fs, yt); break;
            case DCASE(2, 3):
                z.t = b ? jtfolk(jt, CP, jtfolk(jt, RT, fs, FGR(v)), v->fgh[2]) : jtfolk(jt, xt, jtfolk(jt, LF, fs, FGR(v)), v->fgh[2]);
                break;
            case DCASE(2, 4):
                z.t = b ? jtfolk(jt, CP, jtfolk(jt, RT, fs, v->fgh[1]), v->fgh[2]) : jtfolk(jt, xt, jtfolk(jt, LF, fs, FGR(v)), v->fgh[2]);
                break;
            case DCASE(3, 2):
                z.t = b ? jtfolk(jt, CP, jtfolk(jt, FGR(u), fs, RT), yt) : jtfolk(jt, u->fgh[2], jtfolk(jt, FGL(u), fs, RT), yt);
                break;
            case DCASE(3, 3):
                z.t = b ? jtfolk(jt, CP, jtfolk(jt, FGR(u), fs, FGR(v)), v->fgh[2])
                        : jtfolk(jt, u->fgh[2], jtfolk(jt, FGL(u), fs, FGR(v)), v->fgh[2]);
                break;
            case DCASE(3, 4):
                z.t = b ? jtfolk(jt, CP, jtfolk(jt, FGR(u), fs, v->fgh[1]), v->fgh[2])
                        : jtfolk(jt, u->fgh[2], jtfolk(jt, FGL(u), fs, FGR(v)), v->fgh[2]);
                break;
            case DCASE(4, 2):
                z.t = b ? jtfolk(jt, CP, jtfolk(jt, u->fgh[1], fs, RT), yt) : jtfolk(jt, u->fgh[2], jtfolk(jt, FGL(u), fs, RT), yt);
                break;
            case DCASE(4, 3):
                z.t = b ? jtfolk(jt, CP, jtfolk(jt, u->fgh[1], fs, FGR(v)), v->fgh[2])
                        : jtfolk(jt, u->fgh[2], jtfolk(jt, FGL(u), fs, FGR(v)), v->fgh[2]);
                break;
            case DCASE(4, 4):
                z.t = b ? jtfolk(jt, CP, jtfolk(jt, u->fgh[1], fs, v->fgh[1]), v->fgh[2])
                        : jtfolk(jt, u->fgh[2], jtfolk(jt, FGL(u), fs, FGR(v)), v->fgh[2]);
                break;
            case DCASE(0, 5): z.t = jtfolk(jt, v->fgh[0], jtfolk(jt, x.a, fs, v->fgh[1]), v->fgh[2]); break;
            case DCASE(2, 5):
                if (b || c) z.t = jtfolk(jt, v->fgh[0], jtfolk(jt, b ? RT : LF, fs, v->fgh[1]), v->fgh[2]);
                break;
            case DCASE(3, 5):
            case DCASE(4, 5):
                if (b || c) z.t = jtfolk(jt, v->fgh[0], jtfolk(jt, b ? FGR(u) : FGL(u), fs, v->fgh[1]), v->fgh[2]);
                break;
            case DCASE(5, 0): z.t = jtfolk(jt, u->fgh[0], jtfolk(jt, y.a, sf, u->fgh[1]), u->fgh[2]); break;
            case DCASE(5, 2):
                if (b || c) z.t = jtfolk(jt, u->fgh[0], jtfolk(jt, u->fgh[1], fs, b ? RT : LF), yt);
                break;
            case DCASE(5, 3):
            case DCASE(5, 4):
                if (b || c) z.t = jtfolk(jt, u->fgh[0], jtfolk(jt, u->fgh[1], fs, b ? FGR(v) : FGL(v)), v->fgh[2]);
                break;
            case DCASE(5, 5):
                if (xi == yi && xj == yj || xi == yj && xj == yi)
                    if (b || v->fgh[1] == jtswapc(jt, v->fgh[1]))
                        z.t = jtfolk(jt, u->fgh[0], jtfolk(jt, u->fgh[1], fs, v->fgh[1]), u->fgh[2]);
                    else if (u->fgh[1] == jtswapc(jt, u->fgh[1]))
                        z.t = jtfolk(jt, v->fgh[0], jtfolk(jt, u->fgh[1], fs, v->fgh[1]), v->fgh[2]);
                    else
                        z.t = jtfolk(jt, u->fgh[0], jtfolk(jt, u->fgh[1], fs, jtswap(jt, v->fgh[1])), u->fgh[2]);
        }
        RZZ(z.t);
    }
    return z;
}

TA
jtvadv(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0) {
    TA z = {0, 0};
    if (CHK1) df1(z.a, stack[b].a, stack[e].a);
    return z;
}

TA
jtvconj(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0) {
    TA z = {0, 0};
    if (CHK2) df2(z.a, stack[b].a, stack[e].a, stack[e - 1].a);
    return z;
}

TA
jtvfolk(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0) {
    TA z = {0, 0};
    if (CHK3) z.a = jtfolk(jt, stack[b].a, stack[1 + b].a, stack[e].a);
    return z;
}

TA
jtvhook(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0) {
    TA z = {0, 0};
    if (CHK2) z.a = jthook(jt, stack[b].a, stack[e].a);
    return z;
}

TA
jtvpunc(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0) {
    return stack[e - 1];
}

TA
jtvis(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0) {
    A ea, et, n, t;
    I j;
    TA *u, z = {0, 0};
    n = stack[b].a;
    if (!(NAME & AT(n) && CASGN == CAV(stack[1 + b].a)[0])) return z;
    t = jtsfn(jt, 0, n);
    j = *ttabi;
    u = ttab;
    if (!t || NTTAB == *ttabi) return z;
    DQ(j, if (jtequ(jt, t, u->a)) return z; ++u;);
    ea = stack[e].a;
    et = stack[e].t;
    jtsymbisdel(jt, n, ea, locsyms);
    ++*ttabi;
    u->a = t;
    u->t = et ? et : jtcfn(jt, ea);
    z.a  = ea;
    z.t  = tsubst ? jtqq(jt, jtsc(jt, TC + j), num(-1)) : et;
    return z;
}

static TA
jtvmove(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0) {
    A t;
    TA *u, x, z;
    x = stack[MAX(0, e)];
    if (!(NAME & AT(x.a)) || ASGN & AT(stack[b].a)) return x;
    z.a = jtnameref(jt, x.a, locsyms);
    z.t = 0;
    t   = jtsfn(jt, 0, x.a);
    u   = ttab;
    DO(
      *ttabi, if (jtequ(jt, t, u->a)) {
          z.t = tsubst && ttabi0 <= i ? jtqq(jt, jtsc(jt, TC + i), num(-1)) : u->t;
          break;
      } ++u;);
    return z;
}

/* final translator result                             */
/* modifies argument in place                          */
/* a. replaces 880i functions by ttab[i].t entries */
/* b. replaces n0"_ v1 v2 by n0 v1 v2                  */
/* c. replaces [: g ] by g, if monad                   */

static A
jtvfinal(J jt, A w, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0) {
    I i;
    V *u, *v;
    if (!(VERB & AT(w))) return w;
    v = FAV(w);
    if (CFORK != v->id) {
        i = tvi(w);
        return 0 <= i ? jtvfinal(jt, ttab[i].t, tmonad, tsubst, ttab, ttabi, ttabi0) : w;
    }
    RZ(v->fgh[0] = jtincorp(jt, tine(jtvfinal(jt, v->fgh[0], tmonad, tsubst, ttab, ttabi, ttabi0))));
    RZ(v->fgh[1] = jtincorp(jt, tine(jtvfinal(jt, v->fgh[1], tmonad, tsubst, ttab, ttabi, ttabi0))));
    RZ(v->fgh[2] = jtincorp(jt, tine(jtvfinal(jt, v->fgh[2], tmonad, tsubst, ttab, ttabi, ttabi0))));
    if (VERB & AT(v->fgh[0])) {
        u = FAV(v->fgh[0]);
        if (CFCONS == u->id)
            v->fgh[0] = u->fgh[2];  // must be incorped already
        else if (CQQ == u->id && NOUN & AT(u->fgh[0]) && jtequ(jt, ainf, u->fgh[1]))
            v->fgh[0] = u->fgh[0];  // must be incorped already
        if (NOUN & AT(v->fgh[0])) RZ(w = jtfolk(jt, v->fgh[0], v->fgh[1], v->fgh[2]));
    }
    return tine(w);
}

static A
jttparse(J jt, A w, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0) {
    A *s, t, x;
    C d;
    I b, *c, e, i, j, k, m, n;
    TA *stack;
    stack = (TA *)AV(w);
    n = m = AS(w)[0] - 4;
    do {
        for (i = 0; i < NCASES; i++) {
            c = cases[i].c;
            s = (A *)(n + stack);
            d = 1;
            d = d && *c++ & (AT(*s) & -AT(*s));
            s += WTA;
            d = d && *c++ & (AT(*s) & -AT(*s));
            s += WTA;
            d = d && *c++ & (AT(*s) & -AT(*s));
            s += WTA;
            d = d && *c++ & (AT(*s) & -AT(*s));
            if (d) break;
        }
        if (i < NCASES) {
            b        = cases[i].b;
            j        = n + b;
            e        = cases[i].e;
            k        = n + e;
            stack[k] = (cases[i].vf)(jt, j, k, stack, locsyms, tmonad, tsubst, ttab, ttabi, ttabi0);
            if (!(stack[k].a)) return 0;
            DQ(b, stack[--k] = stack[--j];);
            n = k;
        } else {
            stack[n - 1] = jtvmove(jt, n, m - 1, stack, locsyms, tmonad, tsubst, ttab, ttabi, ttabi0);
            RE(0);
            n -= 0 < m--;
        }
    } while (0 <= m);
    x = stack[1 + n].a;
    t = stack[1 + n].t;
    ASSERT(NOUN & AT(x) && MARK & AT(stack[2 + n].a), EVSYNTAX);
    return t ? jtvfinal(jt, t, tmonad, tsubst, ttab, ttabi, ttabi0) : jtcfn(jt, x);
}

A
jtvtrans(J jt, A w) {
    PROLOG(0053);
    A locsyms, y, z = 0;
    I c, i, ttabi;
    TA ttab[NTTAB];
    RZ(ttab[0].a = jtcstr(jt, "x"));
    ttab[0].t = ds(CLEFT);
    RZ(ttab[1].a = jtcstr(jt, "y"));
    ttab[1].t = RT;
    c         = 2;
    for (i = 0; !z && 2 > i; ++i) {
        RZ(y = jtvtokens(jt, w));  // return AM bit0=monad
        I tmonad = AM(y);
        ttabi    = c;
        RZ(locsyms = jtstcreate(jt, 2, 40, 0L, 0L));  // not necessary to set global pointers
        jtsymbis(jt, mnuvxynam[5], jtrue, locsyms);
        if (!tmonad) jtsymbis(jt, mnuvxynam[4], jtrue, locsyms);
        z = jttparse(jt, y, locsyms, tmonad, 0 == i, ttab, &ttabi, c);
        RESETERR;
        if (i && !z) z = jtcolon(jt, numbool(4 - tmonad), w);
    }

    EPILOG(z);
}
